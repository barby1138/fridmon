// element.cpp
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

#include "element.h"
#include <quark/strings.h>

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// elem_exception
//////////////////////////////////////////////////////////////////////

elem_exception::elem_exception(const char* name, const char* descr)
	:	pruntime_error(strings::format("element: %s; %s", name, descr))
{
}

//////////////////////////////////////////////////////////////////////
// element constructor/destructor
//////////////////////////////////////////////////////////////////////

element::~element()
{
	std::for_each(_attributes.begin(), _attributes.end(), cleaner_ptr());
	std::for_each(_elements.begin(), _elements.end(), cleaner_ptr());
}

void element::setIndexItem(const index_item& index) 
{ 
	// init attributes if needed
	getAttributes();

	if (_keyAttributes.size() != index.getKeys().size())
		throw elem_exception(getName(), "wrong number of keys in the index");
	if (strcmp(getName(), index.getName()))
		throw elem_exception(getName(), "wrong index name");

	for (size_t i = 0; i < _keyAttributes.size(); i++)
	{
		_keyAttributes[i]->setValue(index.getKeys()[i].getValue());			
		_keys.push_back(index.getKeys()[i].getValue());
	}
}

//////////////////////////////////////////////////////////////////////
// element modifiers
//////////////////////////////////////////////////////////////////////

// TODO: check if attr exists
void element::addAttribute(attribute* attr)
{ 
	assert(attr);
	getAttributes()[attr->getName()] = attr;
}

void element::addElement(const index_item& ind)
{
	if (findElement(ind))
		throw std::runtime_error("element::addElement element already exists");

	// create element
	pauto_ptr<element> newEntry( getFactory().createObject(ind.getName()) );

	if (newEntry.get())
	{
		// populate keys
		newEntry->setIndexItem(ind);
		addElement(newEntry.release());
	}
}

void element::addElement(element* elem)
{
	assert(elem);
	
	elem->setLoaded();
	
	elements_hash_t& elementsHash = getElementsHash();

	do 
	{
		const index_item idxName = index_item(elem->getName());
		
		// remove not loaded elements with same name
		elements_t& elements = getElements();
		elements_hash_t::iterator itor = elementsHash.lower_bound(idxName);
		elements_hash_t::iterator itorEnd = elementsHash.upper_bound(idxName);
		elements_hash_t::iterator itorNext;
		while (itor != itorEnd)
		{
			elements_t::iterator itEl = itor->second;
			if (!(*itEl)->isLoaded())
			{
				itorNext = itor;
				++itorNext;
				
				delete *itEl;

				elements.erase(itEl);
				elementsHash.erase(itor);

				itor = itorNext;
			}
			else
			{
				++itor;
			}
		}

		// try to find element with full index
		const index_item idx = elem->getIndexItem();
		itor = elementsHash.lower_bound(idx);
		itorEnd = elementsHash.upper_bound(idx);
		while (itor != itorEnd)
		{
			elements_t::iterator itEl = itor->second;
			if (!(*itEl)->isLoaded())
			{
				delete *itEl;
				*itEl = elem;
				elem = NULL;
				break;
			}
			
			++itor;
		}
		
		if (!elem)
			break;

		// add new element
		elements_t::iterator itEl = elements.insert(elements.end(), elem);
		elementsHash.insert(elements_hash_t::value_type(idx, itEl));
	} while(false);
}

unsigned element::removeElements(const index_item& idx)
{
	unsigned uRes = 0;
	
	elements_t& elements = getElements();
	elements_hash_t& elementsHash = getElementsHash();
	
	elements_hash_t::iterator itor = elementsHash.lower_bound(idx);
	elements_hash_t::iterator itorEnd = elementsHash.upper_bound(idx);
	elements_hash_t::iterator itorNext;
	while (itor != itorEnd)
	{
		itorNext = itor;
		++itorNext;
	
		elements_t::iterator itEl = itor->second;
		delete (*itEl);
		elements.erase(itEl);
		elementsHash.erase(itor);
		++uRes;
		
		itor = itorNext;
	}

	return uRes;
}

//////////////////////////////////////////////////////////////////////
// element accessors
//////////////////////////////////////////////////////////////////////

attribute& element::getAttribute(const quark::pstring& name)
{ 
	attribute* attr = findAttribute(name);
	if (!attr)
		throw elem_exception(getName(), 
			strings::format("specified attribute '%s' does not exist", name.c_str()).c_str());
	return *attr;
}

element& element::getElement(const index& idx)
{
	element* elem = findElement(idx);
	if (!elem)
		throw elem_exception(getName(), 
			strings::format("specified element '%s' does not exist", idx.toString().c_str()).c_str());
	return *elem;
}

attribute* element::findAttribute(const quark::pstring& name)
{
	attributes_t::const_iterator itor = getAttributes().find(name);
	return (itor != getAttributes().end()) ? itor->second : NULL;
}

element* element::findElement(const index& idx)
{
	index tmpIdx(idx);

	// check for self
	if (tmpIdx.itemCount() == 0)
		return this;

	// remove self from index
	if (tmpIdx.getItem(0) == getIndexItem())
	{
		tmpIdx.removeItem(0);
		return findElement(tmpIdx);
	}

	elements_hash_t& elementsHash = getElementsHash();

	// find child element
	elements_hash_t::const_iterator itor = elementsHash.find(tmpIdx.getItem(0));
	if (itor == elementsHash.end())
		return NULL;

	return (*itor->second)->findElement(idx);
}

void element::findElements(const index& idx, element::elements_t& elements)
{
	index tmpIdx(idx);
	
	// check for self
	if (tmpIdx.itemCount() == 0)
	{
		elements.push_back(this);
	}
	// remove self from index
	else if (tmpIdx.getItem(0) == getIndexItem())
	{
		tmpIdx.removeItem(0);
		findElements(tmpIdx, elements);
	}
	else
	{
		elements_hash_t& elementsHash = getElementsHash();
		
		// find child elements
		elements_hash_t::iterator itor = elementsHash.lower_bound(tmpIdx.getItem(0));
		elements_hash_t::iterator itorEnd = elementsHash.upper_bound(tmpIdx.getItem(0));
		while (itor != itorEnd)
		{
			(*itor->second)->findElements(idx, elements);
			
			++itor;
		}
	}
}

element::attributes_t& element::getAttributes()
{
	if (_attributes.empty())
		initAttributes(_attributes, _keyAttributes);
	return _attributes;
}

const element::attributes_t& element::getAttributes() const
{
	return const_cast<element*>(this)->getAttributes();
}

element::elements_t& element::getElements()
{
	if (_elements.empty())
		initElements(_elements, _elementsHash, _elementFactory);
	return _elements;
}

const element::elements_t& element::getElements() const
{
	return const_cast<element*>(this)->getElements();
}

element::element_factory_t& element::getFactory()
{
	if (_elements.empty())
		initElements(_elements, _elementsHash, _elementFactory);
	return _elementFactory;
}

element::elements_hash_t& element::getElementsHash()
{
	if (_elements.empty())
		initElements(_elements, _elementsHash, _elementFactory);
	return _elementsHash;
}

//////////////////////////////////////////////////////////////////////
// element serializer/deserializer
//////////////////////////////////////////////////////////////////////

void element::fromStorage(dom_element* domElem)
{
	// set value
	_value = domElem->getValue();

	// populate elements
	const dom_element::elements_t domElements = domElem->getElements();
	for (dom_element::elements_t::const_iterator elemItor = domElements.begin(); 
		 elemItor != domElements.end(); ++elemItor)
	{
		dom_element* domElem = *elemItor;
		pauto_ptr<element> elem( getFactory().createObject(domElem->getName()) );
		if (elem.get())
		{
			elem->fromStorage(domElem);
			addElement(elem.release());
		}
	}
			 
	// populate attributes
	for (attributes_t::const_iterator attrItor = getAttributes().begin(); 
		 attrItor != getAttributes().end(); ++attrItor)
	{
		dom_attribute* domAttr = domElem->getAttribute(attrItor->second->getName());
		if (domAttr)
		{
			attrItor->second->fromStorage(domAttr);
		}
	}

	// populate keys
	for (key_attributes_t::const_iterator keyItor = _keyAttributes.begin();
		 keyItor != _keyAttributes.end(); ++keyItor)
	{
		_keys.push_back(getAttribute((*keyItor)->getName()).getValue());
	}
}

dom_element* element::toStorage(dom_document* domDoc) const
{
	// create a DOM element
	pauto_ptr<dom_element> domElem( domDoc->createElement(getIndexItem().getName()) );

	// set value
	domElem->setValue(_value);
	
	// add child DOM elements
	for (elements_t::const_iterator elemItor = getElements().begin(); 
		 elemItor != getElements().end(); ++elemItor)
	{
		domElem->addChild((*elemItor)->toStorage(domDoc));
	}
	
	// add DOM attributes
	for (attributes_t::const_iterator attrItor = getAttributes().begin(); 
		 attrItor != getAttributes().end(); ++attrItor)
	{
		domElem->addChild(attrItor->second->toStorage(domDoc));
	}
	return domElem.release();
}
