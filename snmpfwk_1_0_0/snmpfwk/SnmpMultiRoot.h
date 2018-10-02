// SnmpMultiRoot.h: interface for the SnmpMultiRoot class.
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

#ifndef SNMPFWK_SNMPMULTIROOT_INC_
#define SNMPFWK_SNMPMULTIROOT_INC_

#include "SnmpNode.h"

#include <functional>

template<>
struct std::greater<snmpfwk::SnmpNode*> 
{
	bool operator()(const snmpfwk::SnmpNode* lsh, const snmpfwk::SnmpNode* rhs) const
	{
		return (lsh->getOid() < rhs->getOid()); 
	}
};

namespace snmpfwk
{

//////////////////////////////////////////////////
// class SnmpMultiRoot
//
class SnmpMultiRoot
{
private:
	typedef quark::plist<SnmpNode*> roots_t;

public:
	SnmpMultiRoot();

	bool registerRoot(SnmpNode& root);
	void reset();
	SnmpNode* getRoot(const Oid& oid) const;
	const Oid getNextOid(const Oid& oid) const;
	bool getNextSupportedRoot(Oid& outOid) const;

private:
	roots_t _roots;
};

} // namespace snmpfwk

#endif // SNMPFWK_SNMPMULTIROOT_INC_
