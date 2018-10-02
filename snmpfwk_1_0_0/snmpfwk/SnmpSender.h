// SnmpSender.h: interface for the SnmpSender class.
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

#ifndef SNMPFWK_SNMPSENDER_INC_
#define SNMPFWK_SNMPSENDER_INC_

#include "SnmpTypes.h"

namespace snmpfwk
{

// forward declaration
class SnmpSenderImpl;

//////////////////////////////////////////////////
// class SnmpSender
//
class SnmpSender
{
public:
	SnmpSender();
	~SnmpSender();

	bool open(const char* community);
	void close();
	bool send(const IpEndPoint& addr, const SnmpParamList& params);

private:
	SnmpSenderImpl* _pImpl;
};

} // namespace snmpfwk

#endif // SNMPFWK_SNMPSENDER_INC_
