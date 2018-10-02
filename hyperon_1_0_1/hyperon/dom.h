// dom.h
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

#ifndef _HYPERON_DOM_H_
#define _HYPERON_DOM_H_

#include <quark/config.h>

namespace hyperon
{

class dom_element;
class dom_attribute;

//////////////////////////////////////////////////
// class dom_node
//
class dom_node : public quark::pooled_object
{
public:
	enum node_type_t
	{ 
		nt_document,
		nt_element,
		nt_attribute
	};

public:
	typedef quark::plist<dom_node*> dom_nodes;

public:
	dom_node(const quark::pstring& name, node_type_t type, dom_node* parent)
		:	_name(name),
			_type(type),
			_parent(parent)
	{}

	dom_node(const quark::pstring& name, node_type_t type, const dom_nodes& children, dom_node* parent)
		:	_name(name),
			_type(type),
			_children(children),
			_parent(parent)
	{}

	virtual ~dom_node()
	{
		std::for_each(_children.begin(), _children.end(), quark::cleaner_ptr());
	}

	const char* getName() const { return _name.c_str(); }
	node_type_t getType() const { return _type; }
	dom_node* getParent() const { return _parent; }
	dom_nodes& getChildren() { return _children; }

	void addChild(dom_node* child) { _children.push_back(child); }

	bool isElement() { return nt_element == _type; }
	bool isAttribute() { return nt_attribute == _type; }

	virtual const quark::pstring toString(const char* indent = "") const = 0;

protected:
	dom_nodes _children;

private:
	quark::pstring _name;
	node_type_t _type;
	dom_node* _parent;
};

//////////////////////////////////////////////////
// class dom_attribute
//
class dom_attribute	: public dom_node
{
public:
	dom_attribute(const quark::pstring& name, const quark::pstring& value, dom_node* parent = 0)
		:	dom_node(name, dom_node::nt_attribute, parent),
			_value(value)
	{}

	const char* getValue() const { return _value.c_str(); }

	virtual const quark::pstring toString(const char* indent = "") const
	{
		return quark::pstring(getName()) + '=' + _value;
	}
	
private:
	quark::pstring _value;
};

//////////////////////////////////////////////////
// class dom_element
//
class dom_element : public dom_node
{
public:
	typedef quark::pvector<dom_element*> elements_t;
	typedef quark::pvector<dom_attribute*> attributes_t;

public:
	dom_element(const quark::pstring& name, dom_node* parent = 0)
		:	dom_node(name, dom_node::nt_element, parent)
	{}

	dom_element(const quark::pstring& name, const dom_nodes& children, dom_node* parent)
		:	dom_node(name, dom_node::nt_element, children, parent)
	{}

	dom_element* getElement(const quark::pstring& name)
	{
		dom_nodes::iterator itor = _children.begin();
		while ((itor != _children.end()) && !(name == (*itor)->getName() && (*itor)->isElement())) ++itor;

		return (itor == _children.end()) ? 0 : static_cast<dom_element*> (*itor);
	}

	dom_attribute* getAttribute(const quark::pstring& name)
	{ 
		dom_nodes::iterator itor = _children.begin();
		while ((itor != _children.end()) && (name != (*itor)->getName() && (*itor)->isAttribute())) ++itor;

		return (itor == _children.end()) ? 0 : static_cast<dom_attribute*> (*itor);
	}

	const elements_t getElements() const
	{
		elements_t res;
		for (dom_nodes::const_iterator itor = _children.begin(); itor != _children.end(); ++itor)
		{
			if ((*itor)->isElement())
				res.push_back(static_cast<dom_element*>(*itor));
		}
		return res;
	}

	const attributes_t getAttributes() const
	{
		attributes_t res;
		for (dom_nodes::const_iterator itor = _children.begin(); itor != _children.end(); ++itor)
		{
			if ((*itor)->isAttribute())
				res.push_back(static_cast<dom_attribute*>(*itor));
		}
		return res;
	}

	virtual const quark::pstring toString(const char* indent = "") const
	{
		quark::pstring res = quark::pstring(getName());// + '\n';
		quark::pstring childPrefix = quark::pstring(indent) + "  ";
		quark::pstring childIndent = quark::pstring(indent) + "   ";

		attributes_t attributes = getAttributes();
		for (attributes_t::const_iterator attrItor = attributes.begin(); 
			 attrItor != attributes.end(); ++attrItor)
		{
			res += '\n' + childPrefix + '-' + (*attrItor)->toString(childIndent.c_str());
		}

		elements_t elements = getElements();
		for (elements_t::const_iterator elemItor = elements.begin(); 
			 elemItor != elements.end(); ++elemItor)
		{
			res += '\n' + childPrefix + '+' + (*elemItor)->toString(childIndent.c_str());
		}
		
		if (_value.size())
			res += '-' + _value;

		return res;
	}

	const char* getValue() const { return _value.c_str(); }
	void setValue(const quark::pstring& value) { _value = value; }

private:
	quark::pstring _value;
};

//////////////////////////////////////////////////
// class dom_document
//
class dom_document : public dom_node
{
public:
	dom_document()
		:	dom_node("document", dom_node::nt_document, 0)
	{}

	~dom_document()
	{}

	dom_element* createElement(const quark::pstring& name)
	{
		return new dom_element(name);
	}

	dom_attribute* createAttribute(const quark::pstring& name, const quark::pstring& value)
	{
		return new dom_attribute(name, value);
	}

	virtual const quark::pstring toString(const char* indent = "") const
	{
		return "";
	}
};

} // namespace hyperon

#endif // _HYPERON_DOM_H_
