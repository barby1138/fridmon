// fsm.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Nucleon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_FSM_H_
#define _NUCLEON_FSM_H_

#include "factory.h"

namespace nucleon
{

//------------------------------------------------------------------------------

class fsm_state : public quark::pooled_object
{
public:
	size_t id() const { return _id; }
	static size_t classId() { return UINT_MAX; }

protected:
	explicit fsm_state(size_t id)
		:	_id(id) {}

private:
	size_t _id;
};

//------------------------------------------------------------------------------

class fsm_timer : public quark::pooled_object
{
public:
	size_t value() const
		{ return _value; }

	void tick(size_t slice)
	{ 
		if (!_suspended)
		{
			if (_elapsed + slice >= _value)
			{
				++_expireCount;
				_elapsed = 0; // auto reset
				_expired = true;
			}
			else
			{
				_elapsed += slice;
				_expired = false;
			}
		}
	}

	bool expired() const
		{ return _expired & !_suspended; }

	size_t expireCount() const
		{ return _expireCount; }

	void reset()
	{ 
		_elapsed = 0;
		_expireCount = 0;
		_suspended = false;
		_expired = false;
	}

	void suspend()
		{ _suspended = true; }

	void resume()
		{ _suspended = false; }

	size_t id() const { return _id; }

protected:
	fsm_timer(size_t id, size_t value)
		:	_id(id),
			_value(value),
			_elapsed(0),
			_expireCount(0),
			_suspended(false),
			_expired(false)
	{
	}
		
private:
	size_t _id;
	size_t _value;
	size_t _elapsed;
	size_t _expireCount;
	bool _suspended;
	bool _expired;
};

//------------------------------------------------------------------------------

// namespace prvt
//{

enum transition_type_t { final_transition, conditional_transition };

struct transition : public quark::pooled_object
{
	transition_type_t type() const 
		{ return _type; }

	size_t getNewStateId()
		{ return _newStateId; }

	virtual void process(fsm_state& state, const void* event, size_t& newStateId) = 0;

protected:
	explicit transition(transition_type_t type, size_t newStateId)
		:	_type(type), _newStateId(newStateId) {}

private:
	transition_type_t _type;
	size_t _newStateId;
};

struct transition_final : transition
{
	transition_final(size_t newStateId)
		:	transition(final_transition, newStateId) {}
};

struct transition_condition : transition
{
	transition_condition(size_t newStateId)
		:	transition(conditional_transition, newStateId), 
			_result(false) {}

	bool result() const 
	{ 
		return _result; 
	}

protected:
	bool _result;
};

template<class _State, class _Arg>
struct transition_actionT : transition_final
{
	typedef void (_State::*fn_type)(_Arg&);
	
	explicit transition_actionT(size_t newStateId, fn_type fn)
		:	transition_final(newStateId), _fn(fn) {}

	
	virtual void process(fsm_state& state, const void* event, size_t& newStateId)
	{
		(static_cast<_State&>(state).*_fn)(*(_Arg*)event);
		newStateId = getNewStateId();
	}
	
private:
	fn_type _fn;
};

template<class _State>
struct transition_actionNoArgT : transition_final
{
	typedef void (_State::*fn_type)();
	
	explicit transition_actionNoArgT(size_t newStateId, fn_type fn)
		:	transition_final(newStateId), _fn(fn) {}


	virtual void process(fsm_state& state, const void* event, size_t& newStateId)
	{
		(static_cast<_State&>(state).*_fn)();
		newStateId = getNewStateId();
	}
	
private:
	fn_type _fn;
};

struct transition_noaction : transition_final
{
	explicit transition_noaction(size_t newStateId)
		:	transition_final(newStateId) {}


	virtual void process(fsm_state& state, const void* event, size_t& newStateId)
	{
		newStateId = getNewStateId();
	}
};

template<class _State, class _Arg>
struct transition_conditionT : transition_condition
{
	typedef bool (_State::*fn_type)(_Arg&);
	
	explicit transition_conditionT(size_t newStateId, fn_type fn)
		:	transition_condition(newStateId), _fn(fn) {}
	

	virtual void process(fsm_state& state, const void* event, size_t& newStateId)
	{
		_result = (static_cast<_State&>(state).*_fn)(*(_Arg*)event);
		if (_result)
			newStateId = getNewStateId();
	}
	
private:
	fn_type _fn;
};

//------------------------------------------------------------------------------

struct fsm_key
{
	fsm_key(size_t stateId, size_t eventId)
		:	_stateId(stateId), _eventId(eventId) {}
	
	bool operator<(const fsm_key& rhs) const
	{
		return _stateId == rhs._stateId ? _eventId < rhs._eventId : _stateId < rhs._stateId;
	}

	bool operator==(const fsm_key& rhs) const
	{
		return _stateId == rhs._stateId && _eventId == rhs._eventId;
	}

	bool operator!=(const fsm_key& rhs) const
	{
		return !operator==(rhs);
	}
	
private:
	size_t _stateId;
	size_t _eventId;
};

//------------------------------------------------------------------------------

class fsm_entry : public quark::pooled_object,
				  public quark::noncopyable
{
private:
	typedef quark::pvector<transition_condition*> contitions_t;

public:
	fsm_entry()
		:	_final(0) {}
	
	~fsm_entry()
	{ 
		std::for_each(_conditions.begin(), _conditions.end(), quark::cleaner_ptr());
		delete _final; 
	}
	
	bool isFinal() const
	{ return _final != 0; }
	
	void setFinal(transition* final)
	{
		assert(!_final && "final transition already defined");
		if (_final)
			throw quark::pruntime_error("static_fsm: final transition already defined");
		// TODO: avoid static_cast if possible
		_final = static_cast<transition_final*>(final);
	}
	
	void addCondition(transition* condition)
	{
		_conditions.push_back(static_cast<transition_condition*>(condition));
	}
	
	void process(fsm_state& state, const void* data, size_t& newStateId)
	{
		contitions_t::iterator itor = _conditions.begin();
		for (; itor != _conditions.end(); ++itor)
		{
			(*itor)->process(state, data, newStateId);
			if ((*itor)->result())
				return;
		}
		
		if (_final)
			_final->process(state, data, newStateId);
	}

private:	
	contitions_t _conditions;
	transition_final* _final;
};

typedef assoc_vector<fsm_key, fsm_entry*> fsm_entries_t;

//------------------------------------------------------------------------------

class fsm_timer_entry : public quark::pooled_object
{
private:
	typedef assoc_vector<size_t, fsm_timer*> timers_t;

public:
	~fsm_timer_entry()
	{ 
		clear();
	}

	void clear()
	{
		std::for_each(_timers.begin(), _timers.end(), quark::cleaner_ptr());
		_timers.clear();
	}
	
	void addTimer(fsm_timer* tm)
	{
		assert(tm);
		_timers[tm->id()] = tm;
	}
	
	fsm_timer* getTimer(size_t id)
	{
		timers_t::iterator itor = _timers.find(id);
		return (itor != _timers.end()) ? itor->second : NULL;
	}

	void resumeTimer(size_t id)
	{
		if (fsm_timer* tm = getTimer(id))
			tm->resume();
	}
	
	void suspendTimer(size_t id)
	{
		if (fsm_timer* tm = getTimer(id))
			tm->suspend();
	}

	void resetTimer(size_t id)
	{
		if (fsm_timer* tm = getTimer(id))
			tm->reset();
	}

	template<class _Fsm, class _Scheme>
	void tick(size_t slice, _Fsm& fsm, _Scheme& scheme)
	{
		for (timers_t::iterator itor = _timers.begin(); itor != _timers.end(); ++itor)
		{
			fsm_timer& timer = *itor->second;
			timer.tick(slice);

			if (timer.expired()) // auto reset
			{
				size_t prevStateId = fsm.getCurState().id();
				fsm.process(scheme.createTimeoutEvent(timer));
				size_t newStateId = fsm.getCurState().id();

				// check if state has been changed
				if (prevStateId != newStateId)
					break;
			}
		}
	}
	
private:
	timers_t _timers;
};

//} // namespace prvt

//------------------------------------------------------------------------------

template<class _BaseState = fsm_state>
class fsm_default_traits
{
public:
	typedef _BaseState state_type;

	template<class _Event>
	struct undress_event
	{
		typedef _Event naked;
	};

public:
	template<class _Event>
	static size_t eventClassId(quark::type2type<_Event>)
		{ return _Event::classId(); }

	template<class _Event>
	static size_t eventId(const _Event& event)
		{ return event.id(); }

	void registerState(size_t classId, product_creator<state_type>* creator)
	{
		_stateFactory.registerProduct(classId, creator);
	}

	void registerTimer(size_t classId, product_creator<fsm_timer>* creator)
	{
		_timerFactory.registerProduct(classId, creator);
	}

	template<class _State>
	void registerState(quark::type2type<_State>)
	{
		_stateFactory.registerProduct(_State::classId(), new product_creatorT<state_type, _State>);
	}

	template<class _Timer>
	void registerTimer(quark::type2type<_Timer>)
	{
		_timerFactory.registerProduct(_Timer::classId(), new product_creatorT<fsm_timer, _Timer>);
	}

	state_type* createState(size_t stateId)
	{
		return _stateFactory.createObject(stateId);
	}

	void deleteState(state_type* state) // throw()
	{
		delete state;
	}

	fsm_timer* createTimer(size_t timerId)
	{
		return _timerFactory.createObject(timerId);
	}

	static fsm_timer& createTimeoutEvent(fsm_timer& timer)
	{
		return timer;
	}

protected:
	~fsm_default_traits() {}

private:
	factory<state_type, size_t, product_creator_adaptor<state_type> > _stateFactory;
	factory<fsm_timer, size_t, product_creator_adaptor<fsm_timer> > _timerFactory;
};

//------------------------------------------------------------------------------

class fsm_default_monitor
{
public:
	void setFsmName(const char* name) { _fsmName = name; }
	const char* getFsmName() const { return _fsmName.c_str(); }

protected:
	~fsm_default_monitor() {}

	template<class _State, class _Event>
	void process(const _State& state, const _Event& event)
	{
	}

	template<class _State, class _Event>
	void unexpected(const _State& state, const _Event& event)
	{
	}

	template<class _State>
	void switchState(const _State& state, const _State& newState)
	{
	}

private:
	quark::pstring _fsmName;
};

//------------------------------------------------------------------------------

class fsm_base_scheme
{
private:
	// TODO: change pmap to hash table
	typedef assoc_vector<size_t, quark::pvector<size_t> > fsm_timers_t;
	typedef assoc_vector<size_t, bool> fsm_events_t;

public:
	virtual ~fsm_base_scheme()
	{
		std::for_each(_entries.begin(), _entries.end(), quark::cleaner_ptr());
	}

	void init()
	{
		if (_entries.empty()) 
			onInit(); 
	}

	fsm_entry* getTransitions(size_t stateId, size_t eventId)
	{
		fsm_entries_t::iterator itor = _entries.find(fsm_key(stateId, eventId));
		return (itor != _entries.end()) ? itor->second : NULL;
	}
	
	bool isEvent(size_t eventId) const
	{
		return _events.find(eventId) != _events.end();
	}

	const quark::pvector<size_t>& getTimers(size_t stateId)
	{
		return _timers[stateId];
	}

	void addTransition(size_t stateId, size_t eventId, transition* trans)
	{
		fsm_key key(stateId, eventId);
		fsm_entries_t::iterator itor = _entries.lower_bound(key);
		if (itor == _entries.end() || itor->first != key)
			itor = _entries.insert(itor, fsm_entries_t::value_type(key, new fsm_entry));

		fsm_entry* entry = itor->second;
		if (trans->type() == final_transition)
			entry->setFinal(trans);
		else
			entry->addCondition(trans);
	}

	void addBaseTransition(size_t stateId, size_t eventId, transition* trans)
	{
		fsm_key key(stateId, eventId);
		fsm_entries_t::iterator itor = _entries.lower_bound(key);
		if (itor == _entries.end() || itor->first != key)
			itor = _entries.insert(itor, fsm_entries_t::value_type(key, new fsm_entry));

		fsm_entry* entry = itor->second;
		if (trans->type() == final_transition && !entry->isFinal())
			entry->setFinal(trans);
		else if (trans->type() == conditional_transition)
			entry->addCondition(trans);
		else
			delete trans;
	}

	void addEvent(size_t eventId)
	{
		_events[eventId] = true;
	}

	void addTimer(size_t stateId, size_t timerId)
	{
		_timers[stateId].push_back(timerId);
	}

protected:
	fsm_base_scheme() {}

private:
	virtual void onInit() {}

private:
	fsm_entries_t _entries;
	fsm_timers_t _timers;
	fsm_events_t _events;
};

//------------------------------------------------------------------------------

template
<
	class _Traits = fsm_default_traits<> 
>
class fsm_scheme : public fsm_base_scheme,
				   public _Traits
{
public:
	typedef _Traits traits_type;

    template<class _Event>
	struct remap_event
	{
		static size_t classId()
		{
			return _Traits::eventClassId(quark::type2type<_Event>());
		}
	};
};

//------------------------------------------------------------------------------

template<class _Traits, class _Monitor>
class fsm_base : public _Monitor
{
public:
	virtual ~fsm_base()
	{
		_scheme.deleteState(_prevState);
		_scheme.deleteState(_curState);
	}

	template<class _Event>
	void process(const _Event& event)
	{
		_Monitor::process(getCurState(), event);

		if (!process(_Traits::eventId(event), &event))
			_Monitor::unexpected(getCurState(), event);
	}

	template<class _Event>
	bool supported(const _Event& event) const
	{
		return _scheme.isEvent(_Traits::eventId(event));
	}

	const typename _Traits::state_type& getCurState() const
	{
		assert(_curState);
		return *_curState;
	}

	void switchState(size_t newStateId)
	{
		assert(_curState);

		if (newStateId == _curState->id())
			return;

		_scheme.deleteState(_prevState);
		_prevState = _curState;
		_curState = createState(newStateId);
		_Monitor::switchState(*_prevState, *_curState);

		// set timers for the current state
		_curTimers.clear();
		const quark::pvector<size_t>& timerIds = _scheme.getTimers(newStateId);
		for (size_t i = 0; i < timerIds.size(); ++i)
			_curTimers.addTimer(_scheme.createTimer(timerIds[i]));
	}

	void timerTick(size_t slice)
	{
		_curTimers.tick(slice, *this, _scheme);
	}

	void startTimer(size_t timerId)
	{
		_curTimers.resumeTimer(timerId);
	}

	void stopTimer(size_t timerId)
	{
		_curTimers.suspendTimer(timerId);
	}

	void restartTimer(size_t timerId)
	{
		_curTimers.resetTimer(timerId);
	}

protected:
	fsm_base(fsm_scheme<_Traits>& scheme, size_t initialState)
		:	_scheme(scheme),
			_curState(NULL),
			_prevState(NULL)
	{
		_scheme.init();
		setCurState(createState(initialState));
	}

	fsm_base(fsm_scheme<_Traits>& scheme)
		:	_scheme(scheme),
			_curState(NULL),
			_prevState(NULL)
	{
		_scheme.init();
	}

	void setCurState(typename _Traits::state_type* state)
	{ 
		_curState = state; 
	}

	bool process(size_t eventId, const void* event)
	{
		fsm_entry* entry = _scheme.getTransitions(getCurState().id(), eventId);
		if (!entry)
			return false;

		size_t prevStateId = getCurState().id();
		size_t newStateId = 0;
		entry->process(*_curState, event, newStateId);

		// check with previous state to allow change state recursively from state itself
		if (newStateId != prevStateId && newStateId != UINT_MAX)
			switchState(newStateId);

		return true;
	}

	typename _Traits::state_type* createState(size_t stateId)
	{
		quark::pauto_ptr<_Traits::state_type> state(_scheme.createState(stateId));
		onCreateState(*state.get());
		return state.release();
	}

private:
	virtual void onCreateState(typename _Traits::state_type& state) {}

private:
	fsm_scheme<_Traits>& _scheme;
	typename _Traits::state_type* _curState;
	typename _Traits::state_type* _prevState;
	fsm_timer_entry _curTimers;
};

//------------------------------------------------------------------------------

template<class _Scheme, class _Monitor = fsm_default_monitor>
class fsm : public fsm_base<typename _Scheme::traits_type, _Monitor>
{
private:
	typedef quark::singleton_holder<_Scheme> scheme_singl;

public:
	explicit fsm(size_t initialState)
		:	fsm_base<_Scheme::traits_type, _Monitor>(scheme_singl::instance(), initialState)
	{
	}

protected:
	// use this ctor from derived fsm
	fsm()
		:	fsm_base<_Scheme::traits_type, _Monitor>(scheme_singl::instance())
	{
	}

	_Scheme& getScheme()
		{ return scheme_singl::instance(); }

	const _Scheme& getScheme() const
		{ return scheme_singl::instance(); } 
};

//------------------------------------------------------------------------------

// namespace prvt
//{

template<class _State, class _Event, class _NewState>
struct base_transitionT
{
	typedef _State state_type;
	typedef _Event event_type;
	typedef _NewState new_state_type;
};

template<class _State, class _Event, class _NewState, void (_State::*_Fn)(_Event&)>
struct transitionT : base_transitionT<_State, _Event, _NewState>
{
	static transition_final* make()
	{
		return new transition_actionT<_State, _Event>(_NewState::classId(), _Fn);
	}
};

template<class _State>
struct void_fn_binder
{
	typedef void (_State::*fn_type)();
	
	template<fn_type _Fn>
	struct fn
	{
		typedef void (_State::*fn_type)();
		static fn_type getPtr() { return _Fn; }
	};
};

template<class _State, class _Event, class _NewState, class _Fn>
struct transitionNoArgT : base_transitionT<_State, _Event, _NewState>
{
	static transition_final* make()
	{
		return new transition_actionNoArgT<_State>(_NewState::classId(), _Fn::getPtr());
	}
};

template<class _State, class _Event, class _NewState>
struct transitionNoActT : base_transitionT<_State, _Event, _NewState>
{
	static transition_final* make()
	{
		return new transition_noaction(_NewState::classId());
	}
};

template<class _State, class _Event, class _NewState, bool (_State::*_Fn)(_Event&)>
struct transitionIfT : base_transitionT<_State, _Event, _NewState>
{
	static transition_condition* make()
	{
		return new transition_conditionT<_State, _Event>(_NewState::classId(), _Fn);
	}
};

//} // namespace prvt

//------------------------------------------------------------------------------

template<class _State, class _Timer>
struct timerT
{
	typedef _State state_type;
	typedef _Timer timer_type;
};

//------------------------------------------------------------------------------

} // namespace nucleon

#endif // _NUCLEON_FSM_H_
