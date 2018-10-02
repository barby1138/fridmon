// static_fsm_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "dynamic_fsm_test.h"
#include "fsm/fsm.h"
#include "fsm/fsm_states.h"
#include <nucleon/dynamic_fsm.h>

using namespace quark;
using namespace nucleon;

//------------------------------------------------------------------------------

class SmscFsmScheme : public nucleon::dynamic_fsm_scheme<SmscTraits<SmscState> >
{
//-----------------------------------------------------------------------------------------------------------------+
//  Transition type             | Current state     | Event                 | Target state      | Action           |
//-----------------------------------------------------------------------------------------------------------------+
BEGIN_TRANSITION_MAP()
	FSM_TRANS_IF_ENTRY          ( SmscIdle,			  MapSendCmdPtr,		  SmscIdle,			  isBadDestination )
	FSM_TRANS_ENTRY             ( SmscIdle,			  MapSendCmdPtr,		  SmscWaitOpenCfm,	  locate		   )
	FSM_TRANS_IF_ENTRY	        ( SmscWaitOpenCfm,	  GsmOpenRespDlgMsgPtr,	  SmscIdle,			  isBadOpenCfm	   )
	FSM_TRANS_NO_ARG_ENTRY	    ( SmscWaitOpenCfm,	  GsmOpenRespDlgMsgPtr,	  SmscWaitSrvAck,	  onOpenCfm		   )
	FSM_TRANS_ENTRY	            ( SmscWaitCloseInd,	  GsmCloseDlgMsgPtr,	  SmscWaitCloseInd,	  onCloseInd	   )
	FSM_TRANS_IF_ENTRY	        ( SmscLocateWaitAck,  GsmSrifsmAckMsgPtr,	  SmscIdle,			  isUserError	   )
	FSM_TRANS_ENTRY	            ( SmscLocateWaitAck,  GsmSrifsmAckMsgPtr,	  SmscWaitOpenCfm,	  located		   )
	FSM_TRANS_IF_ENTRY	        ( SmscSendWaitAck,	  GsmFsmAckMsgPtr,		  SmscIdle,			  isUserError	   )
	FSM_TRANS_IF_ENTRY	        ( SmscSendWaitAck,	  GsmFsmAckMsgPtr,		  SmscWaitOpenCfm,	  isReportNeeded   )
	//FSM_TRANSITION	(	SmscSendWaitAck,	GsmFsmAckMsgPtr,		SmscIdle,			sent			),
	FSM_TRANS_NO_ACT_ENTRY      ( SmscSendWaitAck,	  GsmFsmAckMsgPtr,		  SmscReportWaitAck					   )
	FSM_TRANS_IF_ENTRY	        ( SmscReportWaitAck,  GsmRsmdsAckMsgPtr,	  SmscIdle,			  isUserError	   )
	FSM_TRANS_ENTRY	            ( SmscReportWaitAck,  GsmRsmdsAckMsgPtr,	  SmscIdle,			  reported		   )
	FSM_TRANS_NO_ACT_ENTRY      ( SmscReportWaitAck,  T1,					  SmscIdle							   )
END_TRANSITION_MAP()

BEGIN_BASE_TRANSITION_MAP()
	FSM_BASE_TRANS_IF_ENTRY     ( SmscState,		  MessagePtr,			  SmscIdle,			  isBadFormat	   )
	FSM_BASE_TRANS_NO_ARG_ENTRY ( SmscState,		  GsmUAbortDlgMsgPtr,	  SmscIdle,			  abort			   )
	FSM_BASE_TRANS_ENTRY	    ( SmscState,		  GsmPAbortDlgMsgPtr,	  SmscIdle,			  abort			   )
	FSM_BASE_TRANS_ENTRY	    ( SmscState,		  GsmNoticeDlgMsgPtr,	  SmscIdle,			  abort			   )
	FSM_BASE_TRANS_IF_ENTRY     ( SmscWaitSrvAck,	  GsmSrvMessagePtr,		  SmscIdle,			  isProviderError  )
	FSM_BASE_TRANS_ENTRY	    ( SmscWaitSrvAck,	  GsmSrvMessagePtr,		  SmscWaitCloseInd,	  onSrvAck		   )
END_BASE_TRANSITION_MAP()

BEGIN_TIMER_MAP()
	FSM_TIMER_ENTRY				( SmscReportWaitAck,  T1)
END_TIMER_MAP()
};

//------------------------------------------------------------------------------

class SmscFsm : public fsm<SmscFsmScheme, SmscMonitor>
{
public:
	SmscFsm(SmscContext& context)
		:	_context(context)
	{
		setFsmName("dynamic_fsm");
		setCurState(createState(SmscIdle::classId()));
	}

private:
	virtual void onCreateState(SmscFsmScheme::state_type& state)
	{
		state.setFsm(*this);
		state.setContext(_context);
	}

private:
	SmscContext& _context;
};

//------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// dynamic_fsm_test
//////////////////////////////////////////////////////////////////////

const char* dynamic_fsm_test::getName() const
{
	return "dynamic_fsm_test";
}

void dynamic_fsm_test::execute()
{
	SmscContext context;
	SmscFsm fsm(context);

	_TEST_ASSERT( fsm.supported(GsmFsmAckMsgPtr(new GsmFsmAckMsg)) );

	fsm.process(MapSendCmdPtr(new MapSendCmd));

	fsm.process(GsmOpenRespDlgMsgPtr(new GsmOpenRespDlgMsg));
	fsm.process(GsmSrifsmAckMsgPtr(new GsmSrifsmAckMsg));
	fsm.process(GsmCloseDlgMsgPtr(new GsmCloseDlgMsg));

	fsm.process(GsmOpenRespDlgMsgPtr(new GsmOpenRespDlgMsg));
	fsm.process(GsmFsmAckMsgPtr(new GsmFsmAckMsg));
	fsm.process(GsmCloseDlgMsgPtr(new GsmCloseDlgMsg));

	//fsm.process(GsmUAbortDlgMsgPtr(new GsmUAbortDlgMsg));

	fsm.timerTick(100);
	fsm.timerTick(200);
	fsm.timerTick(200);

	_TEST_ASSERT(fsm.getCurState().id() == SmscIdle::classId());
}
