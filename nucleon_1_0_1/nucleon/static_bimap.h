// static_bimap.h
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

#include "bimap.h"
#include <quark/threads.h>

#ifndef _NUCLEON_STATIC_BIMAP_H_
#define _NUCLEON_STATIC_BIMAP_H_

namespace nucleon
{

//////////////////////////////////////////////////
// class template static_bimap
//
template
<
	typename _Left, 
	typename _Right, 
	class _Host,
	class _ThreadingModel = quark::class_level_lockable<_Host>
>
class static_bimap : quark::noncopyable
{
public:
	typedef typename nucleon::bimap<_Left, _Right> bimap_t;

public:
	inline const _Right& right(const _Left& left) const; // throw(std::logic_error)
	inline const _Left& left(const _Right& right) const; // throw(std::logic_error)
	bool findRight(const _Left& left, _Right& right) const;
	bool findLeft(const _Right& right, _Left& left) const;
	inline bool exists(const _Left& left) const;
	inline size_t size() const;

	// WRK: MSVC++ 6.0 doesn't allow to define a 
	// member function template outside the template class (Q241949)
	template <class _Cont>
	inline void enumerate(_Cont& container) const
	{
		typedef typename bimap_t::value_type bitmap_value_t;
		
		std::transform(_bimap.begin(), _bimap.end(), 
			std::back_inserter(container), 
			quark::select1st<bitmap_value_t>());
	}

protected:
	static_bimap() {}
	~static_bimap() {}

	inline const bimap_t& bimap() const;
	virtual void init(bimap_t& bimap) = 0; // throw(std::logic_error)

private:
	mutable bimap_t _bimap;
};;

//////////////////////////////////////////////////
// class template StaticBimapEx
//
template<
			typename _Left, 
			typename _Right, 
			typename _Value, 
			class _Host,
			class _ThreadingModel = quark::class_level_lockable<_Host>
		>
class static_bimap_ex : public static_bimap<_Left, _Right, _Host, _ThreadingModel>
{
private:
	using static_bimap<_Left, _Right, _Host, _ThreadingModel>::init;

public:
	typedef typename quark::pmap<_Left, _Value> values_t;
	typedef typename static_bimap<_Left, _Right, _Host, _ThreadingModel>::bimap_t bimap_t;

public:
	const _Value& value(const _Left& left) const;

protected:
	inline const values_t& values() const;
	virtual void init(bimap_t& bimapex, values_t& values) = 0; // throw(std::logic_error)

private:
	virtual void init(bimap_t& bimap); // override

private:
	mutable values_t m_values;
};

//////////////////////////////////////////////////////////////////////
// static_bimap implementation
//////////////////////////////////////////////////////////////////////

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
inline const _Right& static_bimap<_Left, _Right, _Host, _ThreadingModel>::right(const _Left& left) const // throw(std::logic_error)
{ 
	return bimap().from[left].get();
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
inline const _Left& static_bimap<_Left, _Right, _Host, _ThreadingModel>::left(const _Right& right) const // throw(std::logic_error)
{
	return bimap().to[right].get(); 
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
bool static_bimap<_Left, _Right, _Host, _ThreadingModel>::findRight(const _Left& left, _Right& right) const
{
	typename bimap_t::from_impl::const_iterator itor = bimap().from.find(left);
	if (itor == bimap().from.end())
		return false;
	right = itor->second;
	return true;
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
bool static_bimap<_Left, _Right, _Host, _ThreadingModel>::findLeft(const _Right& right, _Left& left) const
{
	typename bimap_t::to_impl::const_iterator itor = bimap().to.find(right);
	if (itor == bimap().to.end())
		return false;
	left = itor->second;
	return true;
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
inline size_t static_bimap<_Left, _Right, _Host, _ThreadingModel>::size() const
{ 
	return bimap().size(); 
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
inline bool static_bimap<_Left, _Right, _Host, _ThreadingModel>::exists(const _Left& left) const
{
	return bimap().from.find(left) != bimap().end();
}

template <typename _Left, typename _Right, class _Host, class _ThreadingModel>
inline const typename static_bimap<_Left, _Right, _Host, _ThreadingModel>::bimap_t& 
	static_bimap<_Left, _Right, _Host, _ThreadingModel>::bimap() const
{
	if (_bimap.empty())
	{
		typename _ThreadingModel::lock guard;
		(void)guard;

		if (_bimap.empty())
			const_cast<static_bimap*>(this)->init(_bimap);
	}
	return _bimap;
}

//////////////////////////////////////////////////////////////////////
// static_bimap macroses
//////////////////////////////////////////////////////////////////////

#define BEGIN_BIMAP() \
	virtual void init(bimap_t& bimap) {

#define BIMAP_ENTRY(Left, Right) \
	bimap[Left] = Right;

#define END_BIMAP() }

//////////////////////////////////////////////////////////////////////
// static_bimap_ex implementation
//////////////////////////////////////////////////////////////////////

template <typename _Left, typename _Right, typename _Value, class _Host, class _ThreadingModel>
const _Value& static_bimap_ex<_Left, _Right, _Value, _Host, _ThreadingModel>::value(const _Left& left) const
{
	typename values_t::const_iterator itor = values().find(left);
	if (itor == values().end())
		throw bimap_base::value_not_found();
	return itor->second;
}

template <typename _Left, typename _Right, typename _Value, class _Host, class _ThreadingModel>
inline const typename static_bimap_ex<_Left, _Right, _Value, _Host, _ThreadingModel>::values_t& 
	static_bimap_ex<_Left, _Right, _Value, _Host, _ThreadingModel>::values() const
{
	if (m_values.empty())
	{
		static_bimap<_Left, _Right, _Host>::bimap();
	}
	return m_values;
}

template <typename _Left, typename _Right, typename _Value, class _Host, class _ThreadingModel>
void static_bimap_ex<_Left, _Right, _Value, _Host, _ThreadingModel>::init(bimap_t& bimap) // throw(std::logic_error)
{
	init(bimap, m_values);
}

//////////////////////////////////////////////////////////////////////
// static_bimap_ex macroses
//////////////////////////////////////////////////////////////////////

#define BEGIN_BIMAP_EX() \
virtual void init(bimap_t& bimapex, values_t& values) {

#define BIMAP_ENTRY2(Left, Right, Value) \
	bimapex[Left] = Right; \
	values[Left] = Value;

#define END_BIMAP_EX() }

} // namespace nucleon

#endif // _NUCLEON_STATIC_BIMAP_H_
