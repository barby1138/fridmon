// SnmpMultiRoot.cpp: implementation of the SnmpMultiRoot class.
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

#include "SnmpMultiRoot.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpMultiRoot
//////////////////////////////////////////////////////////////////////

SnmpMultiRoot::SnmpMultiRoot()
{
}

bool SnmpMultiRoot::registerRoot(SnmpNode& root)
{
	_roots.push_back(&root);
	_roots.sort(std::greater<SnmpNode*>());
	//TRACE_DEBUG("SnmpMultiRoot::registerRoot(): %s", root.getOid().toString().c_str());
	return true;
}

SnmpNode* SnmpMultiRoot::getRoot(const Oid& oid) const
{
	for (roots_t::const_iterator it = _roots.begin(); it != _roots.end(); ++it)
	{
		if ((*it)->getOid() > oid || oid.startsWith((*it)->getOid()))
		{
			//TRACE_DEBUG("SnmpMultiRoot::getRoot(): current root: %s", (*it)->getOid().toString().c_str());
			return *it;
		}
	}
	return NULL;
}

const Oid SnmpMultiRoot::getNextOid(const Oid& oid) const
{
	for (roots_t::const_iterator it = _roots.begin(); it != _roots.end(); ++it)
	{
		if (oid.startsWith((*it)->getOid()))
		{
			Oid ret((*it)->getOid().suboid(0, (*it)->getOid().size()-1));
			ret += (*it)->getOid().at((*it)->getOid().size()-1) + 1;
			//TRACE_DEBUG("SnmpMultiRoot::getNextOid(): oid: %s", ret.toString().c_str());
			return ret;
		}
	}
	return oid;
}

bool SnmpMultiRoot::getNextSupportedRoot(Oid& outOid) const
{
	static u32 counter = 0;
	u32 i = counter;
	roots_t::const_iterator it = _roots.begin();
	for (; it != _roots.end() && i; ++it, --i);
	if (it == _roots.end())
		return false;
	++counter;
	outOid = (*it)->getOid();
	//TRACE_DEBUG("SnmpMultiRoot::getNextSupportedRoot(): oid: %s", (*it)->getOid().toString().c_str());
	return true;
}

void SnmpMultiRoot::reset()
{
	_roots.clear();
}
