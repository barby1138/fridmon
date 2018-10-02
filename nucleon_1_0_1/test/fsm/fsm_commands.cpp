// fsm_commands.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "fsm_commands.h"

//------------------------------------------------------------------------------

const char* MapSendCmd::name() const
{
	return "MapSendCmd";
}

//------------------------------------------------------------------------------

const char* MapReceiveAckCmd::name() const
{
	return "MapReceiveAckCmd";
}
