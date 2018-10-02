// SnmpSender.cpp: implementation of the CSnmpSender class.
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

#include "SnmpSender.h"
#ifdef _EPILOGUE
#include "Epilogue/SnmpSenderImpl.h"
#elif _WIN32
#include "win32/SnmpSenderImpl.h"
#else
#error "Unknown platform"
#endif  

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpSender::SnmpSender()
	:	_pImpl(new SnmpSenderImpl())
{
}

SnmpSender::~SnmpSender()
{
	delete _pImpl;
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

bool SnmpSender::open(const char* community)
{
	return _pImpl->open(community);
}

void SnmpSender::close()
{
	_pImpl->close();
}

bool SnmpSender::send(const IpEndPoint& addr, const SnmpParamList& params)
{
	return _pImpl->send(addr, params);
}
