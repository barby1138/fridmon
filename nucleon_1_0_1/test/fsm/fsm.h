// fsm.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef NUCLEON_FSM_TEST_H_
#define NUCLEON_FSM_TEST_H_

#include "fsm_commands.h"
#include "fsm_messages.h"
#include <meson/tracer.h>
#include <nucleon/smart_ptr.h>
#include <nucleon/fsm.h>

//------------------------------------------------------------------------------

typedef nucleon::smart_ptr<Command> CommandPtr;
typedef nucleon::smart_ptr<MapSendCmd> MapSendCmdPtr;

typedef nucleon::smart_ptr<Message> MessagePtr;
typedef nucleon::smart_ptr<GsmSrvMessage> GsmSrvMessagePtr;
typedef nucleon::smart_ptr<GsmOpenRespDlgMsg> GsmOpenRespDlgMsgPtr;
typedef nucleon::smart_ptr<GsmCloseDlgMsg> GsmCloseDlgMsgPtr;
typedef nucleon::smart_ptr<GsmUAbortDlgMsg> GsmUAbortDlgMsgPtr;
typedef nucleon::smart_ptr<GsmPAbortDlgMsg> GsmPAbortDlgMsgPtr;
typedef nucleon::smart_ptr<GsmNoticeDlgMsg> GsmNoticeDlgMsgPtr;
typedef nucleon::smart_ptr<GsmFsmAckMsg> GsmFsmAckMsgPtr;
typedef nucleon::smart_ptr<GsmSrifsmAckMsg> GsmSrifsmAckMsgPtr;
typedef nucleon::smart_ptr<GsmRsmdsAckMsg> GsmRsmdsAckMsgPtr;

//------------------------------------------------------------------------------

class FsmTimer : public nucleon::fsm_timer
{
public:
	// mimicry smart pointer to be compiled in fsm traits
	typedef FsmTimer& pointer_type;

	virtual const char* name() const = 0;

	static size_t classFamily() { return 0; }

protected:
	FsmTimer(size_t id, size_t defValue)
		:	fsm_timer(id, defValue) {}
};

template<class _Host, size_t _Id, size_t _Default>
class FsmTimerT : public FsmTimer
{
public:
	// mimicry smart pointer to be compiled in fsm traits
	typedef _Host class_type;
	typedef _Host pointer_type;

public:
	static size_t classId() { return _Id; }

protected:
	FsmTimerT() : FsmTimer(_Id, _Default) {}
};

enum { t1 };

//------------------------------------------------------------------------------

struct T1 : FsmTimerT<T1, t1, 400>
{
	virtual const char* name() const { return "T1"; }
};

//------------------------------------------------------------------------------

inline const char* getFsmEventName(const FsmTimer& timeout)
{
	return timeout.name();
}

template<class _Event>
inline const char* getFsmEventName(const _Event& event)
{
	return event->name();
}

//------------------------------------------------------------------------------

class SmscMonitor : public nucleon::fsm_default_monitor
{
protected:
	template<class _State, class _Event>
	void process(const _State& state, const _Event& event)
	{
		TRACE_INFO("%s::process() state: %s, event: %s", getFsmName(), state.name(), getFsmEventName(event));
	}

	template<class _State, class _Event>
	void unexpected(const _State& state, const _Event& event)
	{
		TRACE_INFO("%s::unexpected(): state: %s, event: %s", getFsmName(), state.name(), getFsmEventName(event));
	}

	template<class _State>
	void switchState(const _State& state, const _State& newState)
	{
		TRACE_INFO("%s::switchState(): current state: %s, target state: %s", getFsmName(), state.name(), newState.name());
	}
};

//------------------------------------------------------------------------------

class EventIdRemapper
{
private:
	struct command_type { char dummy[1]; };
	struct message_type { char dummy[2]; };	
	struct timeout_type { char dummy[3]; };
	enum { cmd_base = 0, msg_base = 0x20000000, timeout_base = 0x40000000, family_range = 256 };

public:
	template<class _Event>
	static size_t classId(quark::type2type<_Event> type)
	{
		if (_Event::class_type::classFamily() == U32_MAX || _Event::class_type::classId() == U32_MAX)
			return U32_MAX;
		return innerClassId(type, quark::int2type<sizeof(select(make(type)))>());
	}

	template<class _Event>
	static size_t id(_Event& event)
	{
		quark::type2type<_Event> type;
		(void)type;
		return innerId(event, quark::int2type<sizeof(select(make(type)))>());
	}

private:
	template<class _Event>
	static typename _Event::pointer_type make(quark::type2type<_Event>);

	static command_type select(Command* );
	static message_type select(Message* );
	static timeout_type select(const FsmTimer& );
	
	template<class _Event>
	static size_t innerClassId(quark::type2type<_Event>, quark::int2type<1>)
	{
		return cmd_base + family_range * _Event::class_type::classFamily() + _Event::class_type::classId();
	}

	template<class _Event>
	static size_t innerClassId(quark::type2type<_Event>, quark::int2type<2>)
	{
		return msg_base + family_range * _Event::class_type::classFamily() + _Event::class_type::classId();
	}

	template<class _Event>
	static size_t innerClassId(quark::type2type<_Event>, quark::int2type<3>)
	{
		return timeout_base + _Event::class_type::classId();
	}

	template<class _Event>
	static size_t innerId(_Event& cmd, quark::int2type<1>)
	{
		return cmd_base + family_range * cmd->family() + cmd->id();
	}

	template<class _Event>
	static size_t innerId(_Event& msg, quark::int2type<2>)
	{
		return msg_base + family_range * msg->family() + msg->id();
	}

	template<class _Event>
	static size_t innerId(_Event& timeout, quark::int2type<3>)
	{
		return timeout_base + timeout.id();
	}
};

//------------------------------------------------------------------------------

template<class _BaseState>
class SmscTraits : public nucleon::fsm_default_traits<_BaseState>
{
private:
	typedef nucleon::fsm_default_traits<_BaseState> base;

protected:
	template<class _Event>
	struct undress_event
	{
		typedef typename _Event::class_type naked;
	};

	/*template<class _BaseEvent, class _Event>
	struct is_base_event
	{
		enum { result = SUPERSUBCLASS(_BaseEvent::class_type, _Event::class_type) };
	};*/

public:
	template<class _Event>
	static size_t eventClassId(quark::type2type<_Event> type)
	{
		return EventIdRemapper::classId(type);
	}

	template<class _Event>
	static size_t eventId(_Event& event)
	{
		return EventIdRemapper::id(event);
	}

	static FsmTimer& createTimeoutEvent(nucleon::fsm_timer& timer)
	{
		return static_cast<FsmTimer&>(timer);
	}

protected:
	SmscTraits() {}
	~SmscTraits() {}
};

//------------------------------------------------------------------------------

class SmscContext
{

public:
	SmscContext()
		:	_pendingState(0) {}

	void setPendingState(size_t state)
		{ _pendingState = state; }
	size_t getPendingState() const
		{ return _pendingState; }

	void setSrvMessage(GsmSrvMessagePtr& msg)
		{ _spSrvMsg = msg; }
	const GsmSrvMessagePtr& getSrvMessage() const
		{ return _spSrvMsg; }

private:
	size_t _pendingState;
	GsmSrvMessagePtr _spSrvMsg;
};

//------------------------------------------------------------------------------

#endif // NUCLEON_FSM_TEST_H_
