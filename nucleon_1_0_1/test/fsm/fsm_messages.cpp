// fsm_messages.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "fsm_messages.h"

//------------------------------------------------------------------------------

const char* GsmOpenRespDlgMsg::name() const
{
	return "GsmOpenRespDlgMsg";
}

const char* GsmCloseDlgMsg::name() const
{
	return "GsmCloseDlgMsg";
}

const char* GsmUAbortDlgMsg::name() const
{
	return "GsmUAbortDlgMsg";
}

const char* GsmPAbortDlgMsg::name() const
{
	return "GsmPAbortDlgMsg";
}

const char* GsmNoticeDlgMsg::name() const
{
	return "GsmNoticeDlgMsg";
}

//------------------------------------------------------------------------------

const char* GsmFsmAckMsg::name() const
{
	return "GsmFsmAckMsg";
}

const char* GsmSrifsmAckMsg::name() const
{
	return "GsmSrifsmAckMsg";
}

const char* GsmRsmdsAckMsg::name() const
{
	return "GsmRsmdsAckMsg";
}
