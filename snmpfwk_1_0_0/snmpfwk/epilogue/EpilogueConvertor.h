// Convertor.h: interface for the CConvertor class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
Project: SIU520 SNMP Proxy
Module: SNMP Core Framework
Copyright: (c) 2003-2004, RadVision.
License: see License.txt in the top level directory.
*/

#if !defined(AFX_CONVERTOR_H__621F9B38_3426_401A_9434_5B56BDE85608__INCLUDED_)
#define AFX_CONVERTOR_H__621F9B38_3426_401A_9434_5B56BDE85608__INCLUDED_

#include "EpiloguePacket.h"
#include <SnmpTypes.h>

//////////////////////////////////////////////////
// class CEpilogueConvertor
//
//! Converts between Epilogue and inner formats  
/*! 
*/
class CEpilogueConvertor : public noncopyable, private ISnmpAnyVisitor  
{
public:

	static CEpilogueConvertor& Instance();
	bool Convert(const CEpiloguePacket& packet, u32 index, SnmpAny& portAny);
	bool Convert(const SnmpAny& portAny, CEpiloguePacket& packet, u32 index);

private:
	CEpilogueConvertor(){};

	// ISnmpAnyVisitor
	virtual void visit(const s32& val);
	virtual void visit(const u32& val);
	virtual void visit(const pstring& val);
	virtual void visit(const Oid& val);
	virtual void visit(const u8& val);

private:
	CEpiloguePacket* m_pPacket;
	u32 m_index;
};

#endif // !defined(AFX_CONVERTOR_H__621F9B38_3426_401A_9434_5B56BDE85608__INCLUDED_)
