// fsm_states.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "fsm_states.h"
#include <meson/tracer.h>

using namespace nucleon;

//------------------------------------------------------------------------------

SmscState::SmscState(size_t id)
	:	fsm_state(id),
		_fsm(NULL)
{
}

void SmscState::abort()
{
	TRACE_DEBUG("SmscState::abort()");
}

/*void SmscState::abort(GsmUAbortDlgMsgPtr& )
{
	TRACE_DEBUG("SmscState::abort(): GsmUAbortDlgMsg");
	abort();
}*/

void SmscState::abort(GsmPAbortDlgMsgPtr& )
{
	TRACE_DEBUG("SmscState::abort(): GsmUAbortDlgMsg");
	abort();
}

void SmscState::abort(GsmNoticeDlgMsgPtr& )
{
	TRACE_DEBUG("SmscState::abort(): GsmNoticeDlgMsg");
	abort();
}

bool SmscState::isBadFormat(MessagePtr&)
{
	TRACE_DEBUG("SmscState::isBadFormat()");
	return false;
}

void SmscState::setContext(SmscContext& context)
{ 
	_context = &context; 
}

SmscContext& SmscState::getContext()
{ 
	assert(_context);
	return *_context; 
}

//------------------------------------------------------------------------------

const char* SmscIdle::name() const
{ 
	return "SmscIdle"; 
}

bool SmscIdle::isBadDestination(MapSendCmdPtr& cmd)
{
	TRACE_DEBUG("SmscIdle::isBadDestination()");
	return false;
}

void SmscIdle::locate(MapSendCmdPtr& cmd)
{
	TRACE_DEBUG("SmscIdle::locate()");
	getContext().setPendingState(locate_wait_ack);
}

//------------------------------------------------------------------------------

const char* SmscWaitOpenCfm::name() const
{ 
	return "SmscWaitOpenCfm"; 
}

bool SmscWaitOpenCfm::isBadOpenCfm(GsmOpenRespDlgMsgPtr& msg)
{
	TRACE_DEBUG("SmscWaitOpenCfm::isBadOpenCfm()");
	return false;
}

void SmscWaitOpenCfm::onOpenCfm()
{
	TRACE_DEBUG("SmscWaitOpenCfm::onOpenCfm()");
}

//------------------------------------------------------------------------------

const char* SmscWaitSrvAck::name() const
{ 
	return "SmscWaitSrvAck"; 
}

bool SmscWaitSrvAck::isProviderError(GsmSrvMessagePtr& msg)
{
	TRACE_DEBUG("SmscWaitSrvAck::isProviderError()");
	return false;
}

void SmscWaitSrvAck::onSrvAck(GsmSrvMessagePtr& msg)
{
	TRACE_DEBUG("SmscWaitSrvAck::onSrvAck()");
	// save a message
	getContext().setSrvMessage(msg);
}

//------------------------------------------------------------------------------

const char* SmscWaitCloseInd::name() const
{ 
	return "SmscWaitCloseInd"; 
}

void SmscWaitCloseInd::onCloseInd(GsmCloseDlgMsgPtr& msg)
{
	TRACE_DEBUG("SmscWaitCloseInd::onCloseInd()");
	
	SmscContext& context = getContext();
	getFsm().switchState(context.getPendingState());
	getFsm().process(context.getSrvMessage());
}

//------------------------------------------------------------------------------

const char* SmscLocateWaitAck::name() const
{ 
	return "SmscLocateWaitAck"; 
}

bool SmscLocateWaitAck::isUserError(GsmSrifsmAckMsgPtr& )
{	
	TRACE_DEBUG("SmscLocateWaitAck::isUserError()");
	return false;
}

void SmscLocateWaitAck::located(GsmSrifsmAckMsgPtr& )
{
	TRACE_DEBUG("SmscLocateWaitAck::located()");
	getContext().setPendingState(send_wait_ack);
}

//------------------------------------------------------------------------------

const char* SmscSendWaitAck::name() const
{ 
	return "SmscSendWaitAck"; 
}

bool SmscSendWaitAck::isUserError(GsmFsmAckMsgPtr& )
{	
	TRACE_DEBUG("SmscSendWaitAck::isUserError()");
	return false;
}

bool SmscSendWaitAck::isReportNeeded(GsmFsmAckMsgPtr& )
{	
	TRACE_DEBUG("SmscSendWaitAck::isReportNeeded()");
	return false;
}

void SmscSendWaitAck::sent(GsmFsmAckMsgPtr& )
{
	TRACE_DEBUG("SmscSendWaitAck::sent()");
}

//------------------------------------------------------------------------------

const char* SmscReportWaitAck::name() const
{ 
	return "SmscReportWaitAck"; 
}

bool SmscReportWaitAck::isUserError(GsmRsmdsAckMsgPtr& )
{
	TRACE_DEBUG("SmscReportWaitAck::isUserError()");
	return true;
}

void SmscReportWaitAck::reported(GsmRsmdsAckMsgPtr& )
{
	TRACE_DEBUG("SmscReportWaitAck::reported()");
}
