// any.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Hyperon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Refactored from the Boost Library
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _HYPERON_ANY_H_
#define _HYPERON_ANY_H_

#include <quark/config.h>

#include <typeinfo>

namespace hyperon
{

//////////////////////////////////////////////////
// class any
//
class any
{
private:
	template<typename _ValueType>
	friend _ValueType* any_cast(any* );

	class placeholder
	{
	public:
		virtual ~placeholder() {}
		
	public:
		virtual const std::type_info& type() const = 0;
		virtual placeholder* clone() const = 0;
		virtual bool operator==(const placeholder& rhs) const = 0;
		virtual bool operator<(const placeholder& rhs) const = 0;
	};
	
	template<typename _ValueType>
    class holder : public placeholder
	{
	public:
		holder(const _ValueType& value)
			:	_held(value) {}
		
	public:
		virtual const std::type_info& type() const
		{
			return typeid(_ValueType);
		}
		
		virtual placeholder* clone() const
		{
			return new holder(_held);
		}

		virtual bool operator==(const placeholder& rhs) const
		{
			return type() == rhs.type() &&
				_held == static_cast<const holder<_ValueType>& >(rhs)._held;
		}

		virtual bool operator<(const placeholder& rhs) const
		{
			if (type() == rhs.type())
				return _held < static_cast<const holder<_ValueType>& >(rhs)._held;
			return quark::pstring(type().name()) < rhs.type().name();
		}
		
	public:
		_ValueType _held;
	};

public:
	any()
		:	_content(0) {}
	
	template<typename _ValueType>
	any(const _ValueType & value)
		:	_content(new holder<_ValueType>(value))
	{
	}
	
	any(const any& other)
		:	_content(other._content ? other._content->clone() : 0)
	{
	}
	
	~any()
	{
		delete _content;
	}
	
public:
	any& swap(any& rhs)
	{
		std::swap(_content, rhs._content);
		return *this;
	}
	
	template<typename _ValueType>
    any& operator=(const _ValueType& rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}
	
	any& operator=(const any& rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}

	bool operator==(const any& rhs) const
	{
		if (_content && rhs._content)
			return *_content == *rhs._content;
		return _content == rhs._content;
	}

	bool operator!=(const any& rhs) const
	{
		return !operator==(rhs);
	}

	bool operator<(const any& rhs) const
	{
		if (_content && rhs._content)
			return *_content < *rhs._content;
		return _content < rhs._content;
	}

public:
	bool empty() const
	{
		return !_content;
	}
	
	const std::type_info& type() const
	{
		return _content ? _content->type() : typeid(void);
	}
	
private:
	placeholder* _content;
};
	
class bad_any_cast : public std::bad_cast
{
public:
	virtual const char* what() const throw()
	{
		return "bad_any_cast: failed conversion using any_cast";
	}
};

template<typename _ValueType>
_ValueType* any_cast(any * operand)
{
	return operand && operand->type() == typeid(_ValueType) ?
		&static_cast<any::holder<_ValueType>*>(operand->_content)->_held :
		0;
}

template<typename _ValueType>
const _ValueType* any_cast(const any* operand)
{
	return any_cast<_ValueType>(const_cast<any*>(operand));
}

template<typename _ValueType>
_ValueType any_cast(const any& operand)
{
	const _ValueType* result = any_cast<_ValueType>(&operand);
	if (!result)
		throw bad_any_cast();
	return *result;
}

} // namespace hyperon
	
#endif // _HYPERON_ANY_H_
