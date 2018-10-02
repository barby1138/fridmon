// fsm_states.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef NUCLEON_FSM_STATES_TEST_H_
#define NUCLEON_FSM_STATES_TEST_H_

//#include <nucleon/fsm.h>
#include "fsm.h"

//------------------------------------------------------------------------------

enum smsc_state_t
{
	idle = 0,

	wait_open_cfm,
	wait_srv_ack,
	wait_close_ind,

	locate_wait_ack,
	send_wait_ack,
	report_wait_ack,

	//receiving
};

//------------------------------------------------------------------------------

// forward declaration
class SmscContext;

class SmscState : public nucleon::fsm_state
{
private:
	typedef nucleon::fsm_base<SmscTraits<SmscState>, SmscMonitor> fsm_t;

public:
	virtual const char* name() const = 0;

	void abort();

	//void abort(GsmUAbortDlgMsgPtr& );
	void abort(GsmPAbortDlgMsgPtr& );
	void abort(GsmNoticeDlgMsgPtr& );

	bool isBadFormat(MessagePtr& );

	void setContext(SmscContext& context);
	SmscContext& getContext();

	void setFsm(fsm_t& fsm) { _fsm = &fsm; }
	fsm_t& getFsm() { assert(_fsm); return *_fsm; }

protected:
	explicit SmscState(size_t id);

private:
	SmscContext* _context;
	fsm_t* _fsm;
};

template<size_t _Id>
class SmscStateT : public SmscState
{
public:
	SmscStateT() : SmscState(_Id) {}
	static size_t classId() { return _Id; }
};

//------------------------------------------------------------------------------

class SmscOgState : public SmscState
{
protected:
	explicit SmscOgState(size_t id)
		:	SmscState(id) {}
};

template<const size_t _Id>
class SmscOgStateT : public SmscOgState
{
public:
	static size_t classId() { return _Id; }

protected:
	SmscOgStateT() : SmscOgState(_Id) {}
};

//------------------------------------------------------------------------------

class SmscIdle : public SmscStateT<idle>
{
public:
	virtual const char* name() const;

	bool isBadDestination(MapSendCmdPtr& cmd);
	void locate(MapSendCmdPtr& cmd);
	//void receive(const GsmOpenDlgMsg& msg);
};

//------------------------------------------------------------------------------

class SmscWaitOpenCfm : public SmscOgStateT<wait_open_cfm>
{
public:
	virtual const char* name() const;

	bool isBadOpenCfm(GsmOpenRespDlgMsgPtr& msg);
	void onOpenCfm();
};

class SmscWaitSrvAck : public SmscOgStateT<wait_srv_ack>
{
public:
	virtual const char* name() const;

	bool isProviderError(GsmSrvMessagePtr& msg);
	void onSrvAck(GsmSrvMessagePtr& msg);
};

class SmscWaitCloseInd : public SmscOgStateT<wait_close_ind>
{
public:
	virtual const char* name() const;

	void onCloseInd(GsmCloseDlgMsgPtr& msg);
};

//------------------------------------------------------------------------------

struct SmscLocateWaitAck : SmscOgStateT<locate_wait_ack>
{
public:
	virtual const char* name() const;

	bool isUserError(GsmSrifsmAckMsgPtr& );
	void located(GsmSrifsmAckMsgPtr& );
};

//------------------------------------------------------------------------------

class SmscSendWaitAck : public SmscOgStateT<send_wait_ack>
{
public:
	virtual const char* name() const;

	bool isUserError(GsmFsmAckMsgPtr& );
	bool isReportNeeded(GsmFsmAckMsgPtr& );
	void sent(GsmFsmAckMsgPtr& );
};

//------------------------------------------------------------------------------

class SmscReportWaitAck : public SmscOgStateT<report_wait_ack>
{
public:
	virtual const char* name() const;

	bool isUserError(GsmRsmdsAckMsgPtr& );
	void reported(GsmRsmdsAckMsgPtr& );
};

//------------------------------------------------------------------------------

#endif // NUCLEON_FSM_STATES_TEST_H_
