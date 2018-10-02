// dynamic_fsm.h
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

#ifndef _NUCLEON_DYNAMIC_FSM_H_
#define _NUCLEON_DYNAMIC_FSM_H_

#include "factory.h"
#include "fsm.h"
#include "typeinfo.h"

namespace nucleon
{

//------------------------------------------------------------------------------

namespace prvt
{

struct type_holder : quark::pooled_object
{
	explicit type_holder(quark::pooled_object* ptr)
		:	_ptr(ptr) {}

	virtual ~type_holder()
		{ delete _ptr; }

	template<class _Base>
	bool derived(quark::type2type<_Base>)
	{
		return dynamic_cast<_Base*>(_ptr) != 0;
	}

private:
	quark::pooled_object* _ptr;
};

template<class _T>
struct type_holderT : type_holder
{
	type_holderT()
		:	type_holder(new _T) {}
};

} // namespace prvt

//------------------------------------------------------------------------------

template
<
	class _Traits = fsm_default_traits<>
>
class dynamic_fsm_scheme : public fsm_scheme<_Traits>
{
private:
	typedef assoc_vector<size_t, prvt::type_holder*> allstates_t;
	typedef assoc_vector<size_t, prvt::type_holder*> allevents_t;

public:
	dynamic_fsm_scheme()
	{
	}

	virtual ~dynamic_fsm_scheme()
	{
		std::for_each(_allstates.begin(), _allstates.end(), quark::cleaner_ptr());
		std::for_each(_allevents.begin(), _allevents.end(), quark::cleaner_ptr());
	}

	template <class _State>
	void registerState(quark::type2type<_State>)
	{
		_Traits::registerState(_State::classId(), new product_creatorT<state_type, _State>);
	}

	template <class _Event>
	void addEvent(quark::type2type<_Event>)
	{
		addEvent(remap_event<_Event>::classId(), new prvt::type_holderT<_Traits::undress_event<_Event>::naked>());
	}	

	template<class _Transition>
	void addBaseTransition(_Transition)
	{
		quark::pvector<size_t> affectedStates;
		quark::pvector<size_t> affectedEvents;

		// if state a base class?
		size_t stateId = _Transition::state_type::classId();
		if (stateId != UINT_MAX)
		{
			affectedStates.push_back(stateId);
		}

		// if event a base class?
		size_t eventId = remap_event<_Transition::event_type>::classId();
		if (eventId != UINT_MAX)
		{
			typedef _Traits::undress_event<_Transition::event_type>::naked naked_event_type;
			affectedEvents.push_back(eventId);
		}

		// collect all derived states
		for (allstates_t::const_iterator stateItor = _allstates.begin(); stateItor != _allstates.end(); ++stateItor)
		{
			assert(stateItor->second);
			if (stateItor->second->derived(quark::type2type<_Transition::state_type>()))
			{
				size_t stateId = stateItor->first;
				affectedStates.push_back(stateId);
			}
		}

		// collect all derived events
		typedef _Traits::undress_event<_Transition::event_type>::naked naked_event_type;
		for (allevents_t::const_iterator eventItor = _allevents.begin(); eventItor != _allevents.end(); ++eventItor)
		{
			assert(eventItor->second);
			if (eventItor->second->derived(quark::type2type<naked_event_type>()))
			{
				size_t eventId = eventItor->first;
				affectedEvents.push_back(eventId);
			}
		}

		// fill transition matrix
		for (size_t i = 0; i < affectedStates.size(); ++i)
		{
			size_t stateId = affectedStates[i];
			for (size_t j = 0; j < affectedEvents.size(); ++j)
			{
				size_t eventId = affectedEvents[j];
				fsm_scheme<_Traits>::addBaseTransition(stateId, eventId, _Transition::make());
			}
		}
	}

	void addState(size_t stateId, prvt::type_holder* type)
	{
		allstates_t::iterator itor = _allstates.lower_bound(stateId);
		if (itor == _allstates.end() || itor->first != stateId)
			_allstates.insert(itor, allstates_t::value_type(stateId, type));
		else
			delete type;
	}

	void addEvent(size_t eventId, prvt::type_holder* type)
	{
		allevents_t::iterator itor = _allevents.lower_bound(eventId);
		if (itor == _allevents.end() || itor->first != eventId)
		{
			_allevents.insert(itor, allevents_t::value_type(eventId, type));
			fsm_scheme<_Traits>::addEvent(eventId);
		}
		else
		{
			delete type;
		}
	}

	template<class timer>
	void registerTimer(quark::type2type<timer>)
	{
		_Traits::registerTimer(timer::classId(), new product_creatorT<fsm_timer, timer>);
	}

	void addTimer(size_t stateId, size_t timerId)
	{
		fsm_scheme<_Traits>::addTimer(stateId, timerId);
	}

private:
	virtual void onInit()
	{
		initTransitions();
		initBaseTransitions();
		initTimers();
	}

	virtual void initTransitions() {}
	virtual void initBaseTransitions() {}
	virtual void initTimers() {}

private:
	allstates_t _allstates;
	allevents_t _allevents;
};

} // namespace nucleon

//------------------------------------------------------------------------------

#define REGISTER(state, event, newState) \
	addState(state::classId(), new nucleon::prvt::type_holderT<state>()); \
	addState(newState::classId(), new nucleon::prvt::type_holderT<newState>()); \
	registerState(quark::type2type<state>()); \
	registerState(quark::type2type<newState>()); \
	addEvent(quark::type2type<event>()); 
	

#define BEGIN_TRANSITION_MAP() void initTransitions() {
#define END_TRANSITION_MAP() }

#define BEGIN_BASE_TRANSITION_MAP() void initBaseTransitions() {
#define END_BASE_TRANSITION_MAP() }

#define BEGIN_TIMER_MAP() void initTimers() {
#define END_TIMER_MAP() }

#define FSM_TRANS_ENTRY(state, event, newState, action) \
	REGISTER(state, event, newState) \
	addTransition(state::classId(), remap_event<event>::classId(), \
	new nucleon::transition_actionT<state, event>(newState::classId(), &state::action));

#define FSM_TRANS_NO_ARG_ENTRY(state, event, newState, action) \
	REGISTER(state, event, newState) \
	addTransition(state::classId(), remap_event<event>::classId(), \
	new nucleon::transition_actionNoArgT<state>(newState::classId(), nucleon::void_fn_binder<state>::fn<&state::action>::getPtr()));

#define FSM_TRANS_NO_ACT_ENTRY(state, event, newState) \
	REGISTER(state, event, newState) \
	addTransition(state::classId(), remap_event<event>::classId(), \
    new nucleon::transition_noaction(newState::classId()));

#define FSM_TRANS_IF_ENTRY(state, event, newState, condition) \
	REGISTER(state, event, newState) \
	addTransition(state::classId(), remap_event<event>::classId(), \
	new nucleon::transition_conditionT<state, event>(newState::classId(), &state::condition));

#define FSM_TIMER_ENTRY(state, timer) \
	addTimer(state::classId(), timer::classId()); \
	registerTimer(quark::type2type<timer>());	

#define FSM_BASE_TRANS_ENTRY(state, event, newState, action) \
	addBaseTransition(nucleon::transitionT<state, event, newState, &state::action>());

#define FSM_BASE_TRANS_NO_ARG_ENTRY(state, event, newState, action) \
	addBaseTransition(nucleon::transitionNoArgT<state, event, newState, nucleon::void_fn_binder<state>::fn<&state::action> >());

#define FSM_BASE_TRANS_NO_ACT_ENTRY(state, event, newState) \
	addBaseTransition(nucleon::transitionNoActT<state, event, newState>());

#define FSM_BASE_TRANS_IF_ENTRY(state, event, newState, condition) \
	addBaseTransition(nucleon::transitionIfT<state, event, newState, &state::condition>());

//------------------------------------------------------------------------------

#endif // _NUCLEON_DYNAMIC_FSM_H_
