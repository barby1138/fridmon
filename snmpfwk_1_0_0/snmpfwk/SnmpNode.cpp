// SnmpNode.cpp: implementation of the SnmpNode class.
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

#include "SnmpNode.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpNode Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpNode::SnmpNode(const Oid& oid)
	:	_oid(oid)
{
}

SnmpNode::~SnmpNode()
{
	SnmpNode::clear();
}

//////////////////////////////////////////////////////////////////////
// SnmpNode Methods
//////////////////////////////////////////////////////////////////////

u32 SnmpNode::query(u8 request, SnmpParam& param)
{
	quark::critical_scope<> guard(getLock());

	if (request == pduGetNext)
		return doGetNext(param);
	else
		return doGetSet(request, param);
}

void SnmpNode::addObject(ISnmpObject* pObject)
{
	quark::critical_scope<> guard(getLock());

	const Oid& oid = pObject->getOid();
	leaves_t& leaves = getLeaves();
	leaves_t::iterator itor = leaves.lower_bound(oid);
	
	if (itor == leaves.end() || itor->first != oid)
	{
		itor = leaves.insert(itor, std::make_pair(oid, pObject));
	}
	else // already exists
	{
		delete itor->second;
		itor->second = pObject;
	}
}

void SnmpNode::removeObject(const Oid& oid)
{
	quark::critical_scope<> guard(getLock());

	leaves_t& leaves = getLeaves();
	leaves_t::iterator itor = leaves.find(oid);
	if (itor != leaves.end())
	{
		delete itor->second;
		leaves.erase(itor);
	}
}

ISnmpObject* SnmpNode::getObject(const Oid& oid) const
{
	quark::critical_scope<> guard(getLock());

	const leaves_t& leaves = getLeaves();
	leaves_t::const_iterator itor = leaves.find(oid);
	return (itor != leaves.end()) ? itor->second : NULL;
}

void SnmpNode::clear()
{
	quark::critical_scope<> guard(getLock());

	std::for_each(getLeaves().begin(), getLeaves().end(), quark::cleaner_ptr());
	leaves_t tmp;
	getLeaves().swap(tmp);
}

const SnmpNode::leaves_t& SnmpNode::getLeaves() const
{
	return const_cast<SnmpNode*>(this)->getLeaves();
}

SnmpNode::leaves_t& SnmpNode::getLeaves()
{
	if (_leaves.empty())
		initLeaves(_leaves);
	return _leaves;
}

// Get and Set
u32 SnmpNode::doGetSet(u8 request, SnmpParam& param)
{
	u32 res = errNoSuchName;
	Oid origOid = param.name;
	leaves_t::const_iterator itor = findLeaf(param.name);
	
	if (itor != getLeaves().end())
		res = itor->second->query(request, param);
	
	param.name = origOid; // restore original oid
	return res;
}

// GetNext
u32 SnmpNode::doGetNext(SnmpParam& param)
{
	u32 res = errNoSuchName;
	leaves_t::const_iterator itor;
	const leaves_t& leaves = getLeaves();

	// if provided oid is less
	if (param.name < _oid)
	{
		param.name = Oid(); // empty oid
		itor = leaves.begin(); // positioning to the first leaf
	}
	else
	{
		itor = findLeaf(param.name); // find leaf
	}
	
	// get next
	// TODO: consider
	//while (itor != leaves.end() && 
	//	  (res = itor->second->query(pduGetNext, param)) == errNoSuchName) ++itor;
	while (itor != leaves.end() && 
		  (res = itor->second->query(pduGetNext, param)) != errNoError) ++itor;
	
	// TODO: check when oid is outside of this node
	param.name = _oid + param.name; // reconstruct oid
	return res;
}

// Searches the nearest lower leaf and returns suffix oid
SnmpNode::leaves_t::const_iterator SnmpNode::findLeaf(/*IN, OUT*/Oid& oid) const
{
	const leaves_t& leaves = getLeaves();
	leaves_t::const_iterator itor = leaves.end();
	
	// check if passed oid within current node's oid range
	if (oid.startsWith(_oid))
	{
		oid = oid.suboid(_oid.size());
		// find the nearest lower leaf
		itor = leaves.upper_bound(oid);
		if (itor != leaves.begin()) --itor;
	}
	
	return itor;
}

