
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

#ifndef SNMPFWK_SNMPNODE_INC_
#define SNMPFWK_SNMPNODE_INC_

#include "SnmpLeaf.h"
#include <quark/sync.h>

namespace snmpfwk
{

/////////////////////////////////////////////////
// class SnmpNode
//
class SnmpNode : public ISnmpObject,
				 public quark::noncopyable
{
protected:
	typedef quark::pmap<Oid, ISnmpObject*> leaves_t;

public:
	SnmpNode(const Oid& oid);
	virtual ~SnmpNode();

	virtual void initialize() {}
	virtual void uninitialize() {}
	virtual u32 query(u8 request, SnmpParam& param);
	virtual const Oid& getOid() const { return _oid; }
	virtual void clear();

	void addObject(ISnmpObject* pObject);
	void removeObject(const Oid& oid);
	ISnmpObject* getObject(const Oid& oid) const;

protected:
	// Override to initialize leaves on derived instance creation
	virtual void initLeaves(leaves_t& leaves) {}
	const leaves_t& getLeaves() const; // use Lock from callee function
	leaves_t& getLeaves();
	quark::critical_section& getLock() const { return _lock; }

private:
	u32 doGetSet(u8 request, SnmpParam& param);
	u32 doGetNext(SnmpParam& param);
	leaves_t::const_iterator findLeaf(/*IN, OUT*/Oid& oid) const;
	
private:
	Oid _oid;
	leaves_t _leaves;
	mutable quark::critical_section _lock;
};

#define BEGIN_LEAF_MAP(_Host) \
	virtual void initLeaves(leaves_t& leaves) { \
		typedef _Host Host;

#define LEAF_ENTRY(id, type, getFn, setFn) \
		typedef snmpfwk::SnmpLeaf<Host, type, snmpfwk::SnmpStaticBinder<Host, type> > Leaf_t##id; \
		snmpfwk::ISnmpLeaf* pLeaf##id = \
			new Leaf_t##id \
				(*this, snmpfwk::Oid(id) + snmpfwk::Oid(0), \
				(&Host::getFn), \
				(&Host::setFn)); \
		leaves[snmpfwk::Oid(id)] = pLeaf##id;

#define LEAF_ENTRY_GET(id, type, getFn) \
		typedef snmpfwk::SnmpLeaf<Host, type, snmpfwk::SnmpStaticBinder<Host, type> > Leaf_t##id; \
		snmpfwk::ISnmpLeaf* pLeaf##id = \
			new Leaf_t##id(*this, snmpfwk::Oid(id) + snmpfwk::Oid(0), \
				(&Host::getFn), \
				(static_cast<Leaf_t##id::SetFn>(NULL)) ); \
		leaves[snmpfwk::Oid(id)] = pLeaf##id;

#define TABLE_ENTRY(id, type) \
		snmpfwk::SnmpTable* pTable##id = new type(snmpfwk::Oid(id) + snmpfwk::Oid(1)); \
		pTable##id->initialize(); \
		leaves[snmpfwk::Oid(id)] = pTable##id;

#define NODE_ENTRY(id, type) \
		snmpfwk::SnmpNode* pNode##id = new type(snmpfwk::Oid(id) + snmpfwk::Oid(1)); \
		pNode##id->initialize(); \
		leaves[snmpfwk::Oid(id)] = pNode##id;

#define END_LEAF_MAP() }

} // namespace snmpfwk

#endif // SNMPFWK_SNMPNODE_INC_
