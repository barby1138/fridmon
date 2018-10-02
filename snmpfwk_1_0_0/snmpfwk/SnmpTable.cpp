// SnmpTable.cpp: implementation of the SnmpTable class.
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Hyperon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#include "SnmpTable.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpTableSchema Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpTableSchema::SnmpTableSchema(SnmpTable& table)
	:	_table(table)
{
}

SnmpTableSchema::~SnmpTableSchema()
{
	std::for_each(_columns.begin(), _columns.end(), quark::cleaner_ptr());
}

//////////////////////////////////////////////////////////////////////
// SnmpTableSchema Methods
//////////////////////////////////////////////////////////////////////

void SnmpTableSchema::addColumn(ISnmpLeaf* pColumn)
{
	const Oid& oid = pColumn->getOid();
	columns_t::iterator itor = _columns.lower_bound(oid);
	
	if (itor == _columns.end() || itor->first != oid)
	{
		itor = _columns.insert(itor, std::make_pair(oid, pColumn));
	}
	else // already exists
	{
		delete itor->second;
		itor->second = pColumn;
	}
}

void SnmpTableSchema::addColumnCreate(ISnmpLeaf* pColumn)
{
	_createAccessColumns.push_back(pColumn->getOid());
	addColumn(pColumn);
}

void SnmpTableSchema::addRowStatus(ISnmpLeaf* pColumn)
{
	_rowStatusOid = pColumn->getOid();
	addColumn(pColumn);
}

void SnmpTableSchema::newRow(const Oid& index)
{
	columns_t::const_iterator itor = _columns.begin();
	for (; itor != _columns.end(); ++itor)
	{
		ISnmpLeaf* pColumn = itor->second;
		ISnmpLeaf* pLeaf = pColumn->clone(pColumn->getOid() + index);
		_table.addObject(pLeaf);
	}

	if (!_rowStatusOid.empty())
		_table.updateRowStatus(index, enActive);
}

const ISnmpLeaf* SnmpTableSchema::getColumn(size_t pos) const
{
	if (pos >= _columns.size())
		return NULL;
	columns_t::const_iterator itor = _columns.begin();
	std::advance(itor, pos);
	return itor->second;
}

bool SnmpTableSchema::checkForCreateAccess(const Oid& index, const Oid& tableOid) const
{
	oid_vector_t::const_iterator itor = _createAccessColumns.begin();
	for (; itor != _createAccessColumns.end(); ++itor)
	{
		if (index.startsWith(tableOid + (*itor)))
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// SnmpTable Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpTable::SnmpTable(const Oid& oid)
	:	SnmpNode(oid),
		_pSchema(NULL),
		_notifEnabled(true)
{
}

SnmpTable::~SnmpTable()
{
	delete _pSchema;
}

//////////////////////////////////////////////////////////////////////
// SnmpTable Methods
//////////////////////////////////////////////////////////////////////

u32 SnmpTable::query(u8 request, SnmpParam& param)
{
	quark::critical_scope<> guard(getLock());

	// check for the RowStatus column
	if ( (request == pduSet) && (!getSchema().getRowStatusOid().empty()) )
	{
		Oid rowStatusOid = getOid() + getSchema().getRowStatusOid();
		
		if ( (param.name.startsWith(rowStatusOid)) && (param.value.getType() == asnInteger32) )
		{
			// TODO: implement return code according to RFC
			s32 status;
			if (!depackAny(SnmpInt(), param.value, status) || 
				!setRowStatus(param.name.suboid(rowStatusOid.size()), status))
				return errGenError;
			return errNoError;
		}
		else if (getSchema().checkForCreateAccess(param.name, getOid()))
		{
			setRowStatus(param.name.suboid(rowStatusOid.size()), enCreateAndGo);
		}
	}

	return SnmpNode::query(request, param);
}

void SnmpTable::clear()
{
	quark::critical_scope<> guard(getLock());
	
	removeRow(Oid()); // remove all rows
}

bool SnmpTable::addRow(const Oid& index)
{
	quark::critical_scope<> guard(getLock());

	// notify derived class
	if (_notifEnabled && !rowAdding(index))
		return false;
	
	// insert user defined columns
	getSchema().newRow(index);
	return true;
}

void SnmpTable::removeRow(const Oid& index)
{
	quark::critical_scope<> guard(getLock());

	// get column id size
	const ISnmpLeaf* pColumn = getSchema().getColumn(0);
	if (!pColumn) // no columns in the table
		return;
	size_t colOidSize = pColumn->getOid().size();

	// define a map of deleted indexes
	typedef quark::pmap<Oid, bool> trash_t;
	trash_t trash;

	// iterate all leaves
	leaves_t& leaves = SnmpNode::getLeaves();
	//size_t oldLeavesNum = leaves.size();
	for (leaves_t::iterator itor = leaves.begin(); itor != leaves.end(); )
	{
		// extract a pure row index (without the column id)
		const Oid curOid = itor->first.suboid(colOidSize);
		// compare indexes
		if (curOid.startsWith(index))
		{
			// delete the current leaf
			leaves_t::iterator tmp = itor++;
			delete tmp->second;
			leaves.erase(tmp);
			// store row index
			trash.insert(std::make_pair(curOid, true));
		}
		else
		{
			++itor;
		}
	}

	// notify derived class with all deleted rows
	for (trash_t::const_iterator itor2 = trash.begin(); 
		itor2 != trash.end() && _notifEnabled; ++itor2)
	{
		const Oid& removedOid = itor2->first;
		rowDeleted(removedOid);
		
		if (_statusMap.find(removedOid) != _statusMap.end())
			_statusMap.erase(_statusMap.find(removedOid));
	}
}

size_t SnmpTable::getRowCount() const
{
	quark::critical_scope<> guard(getLock());

	if (getSchema().getColumnCount() == 0)
		return 0;
	return getLeaves().size() / getSchema().getColumnCount();
}

void SnmpTable::beginInit()
{
	_notifEnabled = false;
}

void SnmpTable::endInit()
{
	_notifEnabled = true;
}

SnmpTableSchema& SnmpTable::getSchema() const
{
	if (_pSchema == NULL)
	{
		_pSchema = new SnmpTableSchema(const_cast<SnmpTable&>(*this));
		const_cast<SnmpTable*>(this)->initSchema(*_pSchema);
	}
	return *_pSchema;
}

void SnmpTable::updateRowStatus(const Oid& index, s32 status)
{
	_statusMap[index] = status;
}

bool SnmpTable::getRowStatus(const Oid& oid, s32& status)
{
	status_map_t::const_iterator it = _statusMap.find(oid);
	if (it != _statusMap.end())
	{
		status = it->second;
		return true;
	}

	return false;
}

// TODO: check RFC for RowStatus handling
bool SnmpTable::setRowStatus(const Oid& oid, const s32& status)
{
	if (oid.empty())
		return false;
	
	//TRACE_DEBUG("SnmpTable::SetRowStatus(): index=%s, status=%d", 
	//	oid.ToString().c_str(), status);
	if (status == enDestroy)
	{
		removeRow(oid);
		return true;
	}
	else if (status == enCreateAndWait)
	{
		if (getLeaves().find(getSchema().getRowStatusOid() + oid) != getLeaves().end())
			return false; // row exists
		
		if (addRow(oid))
		{
			updateRowStatus(oid, enNotReady);
			return true;
		}
	}
	else if (status == enCreateAndGo)
	{
		// check if row exists
		if (getLeaves().find(getSchema().getRowStatusOid() + oid) != getLeaves().end())
			return false;

		if (addRow(oid))
		{
			updateRowStatus(oid, enActive);
			return true;
		}
	}
	else
	{
		status_map_t::iterator it = _statusMap.find(oid);
		if (it != _statusMap.end())
		{
			it->second = status;
			return true;
		}
	}

	return false;
}
