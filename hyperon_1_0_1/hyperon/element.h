// element.h
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

#ifndef _HYPERON_ELEMENT_H_
#define _HYPERON_ELEMENT_H_

#include "attribute.h"
#include "index.h"
#include <nucleon/factory.h>

namespace hyperon
{

//////////////////////////////////////////////////
// class elem_exception
//
class elem_exception : public quark::pruntime_error
{
public:
	elem_exception(const char* name, const char* descr);
};

//////////////////////////////////////////////////
// class element
//
class element : public quark::pooled_object
{
public:
	typedef quark::pmap<quark::pstring, attribute*> attributes_t;
	typedef quark::plist<element*> elements_t;
	typedef quark::pmultimap<index_item, elements_t::iterator> elements_hash_t;

	typedef quark::pvector<attribute*> key_attributes_t;
	typedef nucleon::factory<
		element, 
		quark::pstring, 
		nucleon::product_creator_adaptor<element>,
		nucleon::nothrow_factory_error> element_factory_t;

public:
	element() 
		:	_isLoaded(false) 
	{}

	virtual ~element();

	const index_item getIndexItem() const { return index_item(getName(), _keys); }

	void addAttribute(attribute* attr);
	void addElement(const index_item& ind);
	unsigned removeElements(const index_item& idx);

	attribute& getAttribute(const quark::pstring& name);
	element& getElement(const index& idx);

	attribute* findAttribute(const quark::pstring& name);
	element* findElement(const index& idx);
	void findElements(const index& idx, elements_t& elements);

	attributes_t& getAttributes();
	const attributes_t& getAttributes() const;

	elements_t& getElements();
	const elements_t& getElements() const;

	virtual void fromStorage(dom_element* domElem);
	virtual dom_element* toStorage(dom_document* domDoc) const;

	const quark::pstring getValue() const { return _value; }
	void setValue(const quark::pstring& value) { _value = value; }

	virtual const char* getName() const = 0;

private:
	void setIndexItem(const index_item& index);
	void addElement(element* elem);

protected:
	virtual void initAttributes(attributes_t& attributes, key_attributes_t& keyAttributes) {}
	virtual void initElements(elements_t& elements, elements_hash_t& elementsHash, element_factory_t& factory) {}

	element_factory_t& getFactory();
	elements_hash_t& getElementsHash();

	bool isLoaded() const { return _isLoaded; } 
	void setLoaded() { _isLoaded = true; }

protected:
	quark::pstring	_value;
	attributes_t _attributes;
	key_attributes_t _keyAttributes;
	elements_t _elements;
	elements_hash_t _elementsHash;
	element_factory_t _elementFactory;
	keys_t _keys;
	bool _isLoaded;
};

//////////////////////////////////////////////////
// macros
//////////////////////////////////////////////////

#define DECLARE_ELEMENT(name) \
	static const char* getClassName() { return name; } \
	virtual const char* getName() const { return name; }

#define BEGIN_ELEMENT_MAP() \
	virtual void initElements(elements_t& elements, elements_hash_t& elementsHash, element_factory_t& factory) {

#define MANDATORY_ELEMENT_ENTRY(type) \
		factory.registerProduct(type::getClassName(), \
			nucleon::product_creator_adaptor<element> \
			(new nucleon::product_creatorT<element, type>)); \
		elements_t::iterator itEl##type = elements.insert(elements.end(), new type); \
		elementsHash.insert(elements_hash_t::value_type(index_item(type::getClassName()), itEl##type));

#define OPTIONAL_ELEMENT_ENTRY(type) \
		factory.registerProduct(type::getClassName(), \
			nucleon::product_creator_adaptor<element> \
			(new nucleon::product_creatorT<element, type>));

#define END_ELEMENT_MAP() }

#define BEGIN_ATTRIBUTE_MAP() \
	virtual void initAttributes(attributes_t& attributes, key_attributes_t& keyAttributes) {

#define MANDATORY_ATTRIBUTE_ENTRY(type, name) \
		attributes[name] = new type(name);

#define INITIALIZED_ATTRIBUTE_ENTRY(type, name, value) \
		attributes[name] = new type(name, value);

#define KEY_ATTRIBUTE_ENTRY(type, name) \
		attributes[name] = new type(name); \
		keyAttributes.push_back(attributes[name]);

#define END_ATTRIBUTE_MAP() }

} // namespace hyperon

#endif // _HYPERON_ELEMENT_H_
