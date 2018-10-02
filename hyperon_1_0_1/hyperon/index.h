// index.h
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

#ifndef _HYPERON_INDEX_H_
#define _HYPERON_INDEX_H_

#include "any.h"
#include <quark/strings.h>

namespace hyperon
{

//////////////////////////////////////////////////
// class key_t
//
class key_t
{
public:
	template<class _T>
	key_t(const _T& value)
		:	_value(value) {}

	key_t(const any& value)
		:	_value(value) {}
	
	bool operator<(const key_t& rhs) const
	{
		return _value < rhs._value;
	}

	bool operator==(const key_t& rhs) const
	{
		return _value == rhs._value;
	}

	const any getValue() const
	{
		return _value;
	}

private:
	any _value;
};

typedef quark::pvector<key_t> keys_t;

//////////////////////////////////////////////////
// class index_item
//
class index_item
{
public:
	index_item(const char* name)
		:	_name(name)
	{
	}

	index_item(const char* name, const key_t& key)
		:	_name(name),
			_keys(1, key)
	{
	}

	index_item(const char* name, const keys_t& keys)
		:	_name(name),
			_keys(keys)
	{
	}

	void addKey(const key_t& key)
	{
		_keys.push_back(key);
	}

	void removeItem(size_t num)
	{
		if (num < _keys.size())
			_keys.erase(_keys.begin() + num);
	}

	const key_t& getKey(size_t num) const
	{
		if (num >= _keys.size())
			throw quark::pruntime_error("key number is out of range");
		return *(_keys.begin() + num);
	}

	const keys_t& getKeys() const
	{
		return _keys;
	}

	size_t keyCount() const
	{
		return _keys.size();
	}

	bool operator==(const index_item& rhs) const
	{
		return _name == rhs._name &&
			_keys == rhs._keys;
	}
	
	bool operator<(const index_item& rhs) const
	{
		if (_name == rhs._name)
			return _keys < rhs._keys;
		else
			return _name < rhs._name;
	}
	
	const char* getName() const
		{ return _name.c_str();	}

	const quark::pstring toString() const;
	
private:
	quark::pstring _name;
	keys_t _keys;
};

//////////////////////////////////////////////////
// class index
//
class index
{
private:
	typedef quark::pvector<index_item> items_t;

public:
	index(const char* str)
	{
		typedef quark::pvector<quark::pstring> strings_t;
		strings_t strings;
		quark::strings::split(quark::pstring(str), ".", std::back_inserter(strings));

		for (strings_t::const_iterator itor = strings.begin(); itor != strings.end(); ++itor)
		{
			addItem(itor->c_str());
		}
	}

	index(const items_t& items)
		:	_items(items)
	{
	}

	index(const index_item& item)
	{
		addItem(item);
	}

	void addItem(const index_item& item)
	{
		_items.push_back(item);
	}

	void removeItem(size_t num)
	{
		if (num < _items.size())
			_items.erase(_items.begin() + num);
	}

	const index_item& getItem(size_t num) const
	{
		if (num >= _items.size())
			throw quark::pruntime_error("item number is out of range");
		return *(_items.begin() + num);
	}

	index_item& getItem(size_t num)
	{
		if (num >= _items.size())
			throw quark::pruntime_error("item number is out of range");
		return *(_items.begin() + num);
	}

	size_t itemCount() const
	{
		return _items.size();
	}

	bool operator==(const index& rhs) const
	{
		return _items == rhs._items; 
	}
	
	bool operator<(const index& rhs) const
	{
		return _items < rhs._items; 
	}

	const quark::pstring toString() const;
	
private:
	items_t _items;
};

inline const index make_index(const char* str, const key_t& lastKey)
{
	index res(str);
	if (res.itemCount() > 0)
		res.getItem(res.itemCount() - 1).addKey(lastKey);
	return res;
}

inline const index make_index(const char* str, const keys_t& lastKey)
{
	index res(str);
	if (res.itemCount() > 0)
	{	
		index_item& idx = res.getItem(res.itemCount() - 1);
		
		keys_t::const_iterator it = lastKey.begin();
		for (; it != lastKey.end(); ++it)
		{
			idx.addKey(*it);
		}
	}
	
	return res;
}


} // namespace hyperon

#endif // _HYPERON_INDEX_H_
