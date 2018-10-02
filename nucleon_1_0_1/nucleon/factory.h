// factory.h
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

////////////////////////////////////////////////////////////////////////////////
// Refactored from the Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_FACTORY_H_
#define _NUCLEON_FACTORY_H_

#include "assoc_vector.h"
#include <quark/mempool.h>

namespace nucleon
{

//////////////////////////////////////////////////
// class throw_factory_error
//
//! Exception throwing factory policy
/*!
	Manages the "Unknown Type" error in an object factory
*/
struct throw_factory_error
{
	template<class _AbstractProduct, typename _IdentifierType>
	static _AbstractProduct* onUnknownType(const _IdentifierType& ,
		quark::type2type<_AbstractProduct> )
	{
		throw quark::pruntime_error("factory: unknown type");
	}
};

//////////////////////////////////////////////////
// class nothrow_factory_error
//
//! No exception throwing factory policy
/*!
	Manages the "Unknown Type" error in an object factory
*/
struct nothrow_factory_error
{
	template<class _AbstractProduct, typename _IdentifierType>
		static _AbstractProduct* onUnknownType(const _IdentifierType& ,
			quark::type2type<_AbstractProduct> )
	{
		return NULL;
	}
};

//////////////////////////////////////////////////
// class template factory
//
//! Generic object factory
/*!
	Implements a generic object factory
*/
template
<
	class _AbstractProduct, 
	typename _IdentifierType,
	typename _ProductCreator = _AbstractProduct* (*)(),
	class _FactoryErrorPolicy = throw_factory_error
>
class factory : protected _FactoryErrorPolicy
{
public:
    bool registerProduct(const _IdentifierType& id, _ProductCreator creator)
    {
        return _associations.insert(
            associations_value_t(id, creator)).second;
    }
    
    bool unregisterProduct(const _IdentifierType& id)
    {
        return _associations.erase(id) == 1;
    }
    
    _AbstractProduct* createObject(const _IdentifierType& id)
    {
        typename associations_t::iterator itor = _associations.find(id);
        if (itor != _associations.end())
        {
            return (itor->second)();
        }
        return onUnknownType(id, quark::type2type<_AbstractProduct>());
    }
    
private:
	typedef assoc_vector<_IdentifierType, _ProductCreator> associations_t;
	typedef typename associations_t::value_type associations_value_t;
    associations_t _associations;
};

//////////////////////////////////////////////////
// class template clone factory
//
//! Generic object clone factory
/*!
	Implements a generic object factory
*/
template
<
	class _AbstractProduct, 
	typename _ProductCreator = _AbstractProduct* (*)(_AbstractProduct*),
	class _FactoryErrorPolicy = throw_factory_error
>
class clone_factory : protected _FactoryErrorPolicy
{
public:
    bool registerProduct(const std::type_info& ti, _ProductCreator creator)
    {
        return _associations.insert(
            associations_t::value_type(ti, creator)).second;
    }
    
    bool unregisterProduct(const std::type_info& ti)
    {
        return _associations.erase(ti) == 1;
    }
    
    _AbstractProduct* createObject(const _AbstractProduct* model)
    {
		typename associations_t::iterator itor = _associations.find(typeid(*model));
        if (itor != _associations.end())
        {
            return (itor->second)(model);
        }
        return onUnknownType(typeid(*model), quark::type2type<_AbstractProduct>());
    }
    
private:
	typedef assoc_vector<std::type_info, _ProductCreator> associations_t;
    associations_t _associations;
};

//////////////////////////////////////////////////
// interface product_creator
//
//! Product creator interface
/*!
	The abstract interface for the concrete product creator functor
*/
template<class _AbstractProduct>
struct product_creator : public quark::pooled_object
{
	virtual ~product_creator() {}
	virtual _AbstractProduct* operator()(void) const = 0;
};

//////////////////////////////////////////////////
// class template product_creator
//
//! Generic product creator functor
/*!
	The concrete product creator functor
*/
template<class _AbstractProduct, class _ConcreteProduct>
struct product_creatorT : product_creator<_AbstractProduct>
{
	virtual _AbstractProduct* operator()(void) const
	{
		return new _ConcreteProduct;
	}
};

//////////////////////////////////////////////////
// class template product_creator_adaptor
//
//! Product creator functor adaptor
/*!
	Adapts a virtual product creator functor to be used as
	a scalar functor
*/
template<class _AbstractProduct>
struct product_creator_adaptor
{
	product_creator_adaptor(product_creator<_AbstractProduct>* p)
		:	_p(p) {}
	
	product_creator_adaptor(const product_creator_adaptor& rhs)
		:	_p(rhs._p) {}
	
	_AbstractProduct* operator()(void) const
		{ return (*_p)(); }

private:
	mutable quark::pauto_ptr<product_creator<_AbstractProduct> > _p;
};

//////////////////////////////////////////////////
// class template static_factory
//
//! Generic static object factory
/*!
	Implements a generic static object factory
*/
template
<
	class _AbstractProduct, 
	typename _IdentifierType, 
	class _FactoryErrorPolicy = throw_factory_error,
	class _ThreadingModel = quark::class_level_lockable<_AbstractProduct>
>
class static_factory
{
protected:
	typedef _AbstractProduct product_type;
	typedef product_creator_adaptor<product_type> adaptor_type;

public:
	_AbstractProduct* createObject(const _IdentifierType& id)
	{
		if (!_initialized)
		{
			typename _ThreadingModel::lock guard;
			(void)guard;

			if (!_initialized)
				initEntries();
		}

		return _factory.createObject(id);
	}
	
protected:
	static_factory()
		:	_initialized(false) {}

	virtual void initEntries() = 0;
	
protected:
	factory<product_type, _IdentifierType, 
		    adaptor_type, _FactoryErrorPolicy > _factory;
	bool _initialized;
};

// macroses
#define BEGIN_TYPE_MAP() \
	virtual void initEntries() { _initialized = true;

#define TYPE_ENTRY(Identifier, ConcreteProduct) \
	_factory.registerProduct(Identifier, \
		nucleon::product_creator_adaptor<product_type> \
		(new nucleon::product_creatorT<product_type, ConcreteProduct>));

#define END_TYPE_MAP() }

} // namespace nucleon

#endif // _NUCLEON_FACTORY_H_
