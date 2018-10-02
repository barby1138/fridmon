// SnmpTable.h: interface for the CSnmpTable class.
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

#ifndef SNMPFWK_SNMPTABLE_INC_
#define SNMPFWK_SNMPTABLE_INC_

namespace snmpfwk
{

//////////////////////////////////////////////////
// enum ERowStatus 
//
enum ERowStatus 
{
   // the following two values are states and actions: these values may be read or written
   enActive			= 1, // state/action (read-write) row is available
   enNotInService	= 2, // state/action (read-write) row exists, but it is not available
   // the following one is state: this value may be read, but not written
   enNotReady		= 3, // state (read) row exists, but missing all info
   // the following three values are actions: these values may be written, but never read
   enCreateAndGo	= 4, // action (write) create new row, and set status to 'active'
   enCreateAndWait	= 5, // action (write) create new row, but do not make it available
   enDestroy		= 6  // action (write) delete row
};

// forward declaration
class SnmpTable;

//////////////////////////////////////////////////
// class SnmpTableSchema
//
class SnmpTableSchema
{
private:
	friend class SnmpTable;
	typedef quark::pmap<Oid, ISnmpLeaf*> columns_t;
	typedef quark::pvector<Oid> oid_vector_t;

public:
	void addColumn(ISnmpLeaf* pColumn);
	void addColumnCreate(ISnmpLeaf* pColumn);
	void addRowStatus(ISnmpLeaf* pColumn);
	void newRow(const Oid& index);

	size_t getColumnCount() const 
		{ return _columns.size(); }
	const Oid getRowStatusOid() const 
		{ return _rowStatusOid; }
	const ISnmpLeaf* getColumn(size_t pos) const;

	bool checkForCreateAccess(const Oid& index, const Oid& tableOid) const;

private:
	SnmpTableSchema(SnmpTable& table);
	~SnmpTableSchema();

private:
	columns_t _columns;
	SnmpTable& _table;
	Oid _rowStatusOid;
	oid_vector_t _createAccessColumns;
};

//////////////////////////////////////////////////
// class SnmpTable
//
class SnmpTable : public SnmpNode
{
private:
	typedef quark::pmap<Oid, s32> status_map_t;

public:
	virtual u32 query(u8 request, SnmpParam& param);
	virtual void clear();

	bool addRow(const Oid& index);
	void updateRowStatus(const Oid& index, s32 status);
	void removeRow(const Oid& index);
	size_t getRowCount() const;

	// Turns off/on notifications while adding/removing rows
	void beginInit();
	void endInit();
	
protected:
	SnmpTable(const Oid& oid);
	virtual ~SnmpTable();

	SnmpTableSchema& getSchema() const;
	virtual void initSchema(SnmpTableSchema& schema) = 0;

	// Events
	virtual bool rowAdding(const Oid& oid) { return true; }
	virtual void rowDeleted(const Oid& oid) {}

	// The conceptual rows management
	bool getRowStatus(const Oid& oid, s32& status);
	bool setRowStatus(const Oid& oid, const s32& status);

private:
	mutable SnmpTableSchema* _pSchema;
	bool _notifEnabled;

	status_map_t _statusMap;
};

#define BEGIN_COLUMN_MAP(_Host) \
	bool innerGetRowStatus(const Index& index, s32& status) \
		{ return getRowStatus(index.toOid(), status); } \
		\
	bool innerSetRowStatus(const Index& index, const s32& status) \
		{ return setRowStatus(index.toOid(), status); } \
		\
	virtual void initSchema(snmpfwk::SnmpTableSchema& schema) { \
		typedef _Host Host; \

#define COLUMN_ENTRY(id, type, getFn, setFn) \
		typedef snmpfwk::SnmpLeaf<Host, type, snmpfwk::SnmpColumnBinder<Host, type, Index> > Entry_t##id; \
		snmpfwk::ISnmpLeaf* pColumn##id = \
			new  Entry_t##id\
				(*this, snmpfwk::Oid(id), \
				(&Host::getFn), \
				(&Host::setFn)); \
		schema.addColumn(pColumn##id);

#define COLUMN_ENTRY_GET(id, type, getFn) \
		typedef snmpfwk::SnmpLeaf<Host, type, snmpfwk::SnmpColumnBinder<Host, type, Index> > Entry_t##id; \
		snmpfwk::ISnmpLeaf* pColumn##id = \
			new  Entry_t##id\
				(*this, snmpfwk::Oid(id), \
				(&Host::getFn), \
				(static_cast<Entry_t##id::SetFn>(NULL)) ); \
		schema.addColumn(pColumn##id);

#define COLUMN_ENTRY_CREATE(id, type, getFn, setFn) \
	typedef snmpfwk::SnmpLeaf<Host, type, snmpfwk::SnmpColumnBinder<Host, type, Index> > Entry_t##id; \
	snmpfwk::ISnmpLeaf* pColumn##id = \
	new  Entry_t##id\
				(*this, snmpfwk::Oid(id), \
				(&Host::getFn), \
				(&Host::setFn)); \
		schema.addColumnCreate(pColumn##id);

#define ROWSTATUS_ENTRY(id) \
		snmpfwk::ISnmpLeaf* pColumn##id = \
			new snmpfwk::SnmpLeaf<Host, snmpfwk::SnmpInt, snmpfwk::SnmpColumnBinder<Host, snmpfwk::SnmpInt, Index> > \
				(*this, snmpfwk::Oid(id), \
				(&Host::innerGetRowStatus), \
				(&Host::innerSetRowStatus)); \
		schema.addRowStatus(pColumn##id); \

#define END_COLUMN_MAP() }

//////////////////////////////////////////////////
// class template TableIndex
//
template<class _Host, const size_t _Size>
class TableIndex
{
public:
	enum { size = _Size };

public:
	TableIndex() {}
	
	const Oid toOid() const 
		{ return Oid(MAKE_OID(_index)); }

	static const _Host fromOid(const Oid& oid);

protected:
	u32 _index[_Size];
};

template<class _Host, const size_t _Size>
const _Host TableIndex<_Host, _Size>::fromOid(const Oid& oid)
{
	_Host host;
	if (oid.size() != size)
		throw quark::pruntime_error("oid to index conversion failed");
	for (size_t i = 0; i < size; ++i)
		host._index[i] = oid.at(i);
	return host;
}

} // namespace snmpfwk

#endif // SNMPFWK_SNMPTABLE_INC_
