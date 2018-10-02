// assoc_vector.h
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

#ifndef _NUCLEON_ASSOC_VECTOR_H_
#define _NUCLEON_ASSOC_VECTOR_H_

#include <quark/config.h>

namespace nucleon
{

namespace prvt
{

//////////////////////////////////////////////////
// class template assoc_vector_compare
//
//! Used by assoc_vector
/*!
*/
template<class _Value, class _C>
	class assoc_vector_compare : public _C
{
private:
    typedef std::pair<typename _C::first_argument_type, _Value> data_type;
    typedef typename _C::first_argument_type first_argument_type;
	
public:
    assoc_vector_compare() {}
    assoc_vector_compare(const _C& src) : _C(src) {}
    
    bool operator()(const first_argument_type& lhs, const first_argument_type& rhs) const
		{ return _C::operator()(lhs, rhs); }
    
    bool operator()(const data_type& lhs, const data_type& rhs) const
		{ return operator()(lhs.first, rhs.first); }
    
    bool operator()(const data_type& lhs, const first_argument_type& rhs) const
		{ return operator()(lhs.first, rhs); }
    
    bool operator()(const first_argument_type& lhs, const data_type& rhs) const
		{ return operator()(lhs, rhs.first); }
};

} // namespace prvt

//////////////////////////////////////////////////
// class template assoc_vector
//
//! An associative vector built as a syntactic drop-in replacement for std::map
/*!
	BEWARE: assoc_vector doesn't respect all map's guarantees, the most important
	        being:
	* iterators are invalidated by insert and erase operations
	* the complexity of insert/erase is O(N) not O(log N)
	* value_type is std::pair<K, V> not std::pair<const K, V>
	* iterators are random
*/
template
<
    class _K,
    class _V,
    class _C = std::less<_K>,
	class _A = quark::allocator<std::pair<_K, _V>, quark::user_mempool>
>
class assoc_vector : private std::vector< std::pair<_K, _V>, _A >,
					 private prvt::assoc_vector_compare<_V, _C>
{
private:
    typedef std::vector<std::pair<_K, _V>, _A> base;
    typedef prvt::assoc_vector_compare<_V, _C> my_compare;

public:
    typedef _K key_type;
    typedef _V mapped_type;
    typedef typename base::value_type value_type;

    typedef _C key_compare;
    typedef _A allocator_type;
    typedef typename _A::reference reference;
    typedef typename _A::const_reference const_reference;
    typedef typename base::iterator iterator;
    typedef typename base::const_iterator const_iterator;
    typedef typename base::size_type size_type;
    typedef typename base::difference_type difference_type;
    typedef typename _A::pointer pointer;
    typedef typename _A::const_pointer const_pointer;
    typedef typename base::reverse_iterator reverse_iterator;
    typedef typename base::const_reverse_iterator const_reverse_iterator;

    class value_compare : public std::binary_function<value_type, value_type, bool>,
						  private key_compare
    {
        friend class assoc_vector;
    
    protected:
        value_compare(key_compare pred)
			:	key_compare(pred) {}

    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const
			{ return key_compare::operator()(lhs.first, rhs.first); }
    };
    
    // 23.3.1.1 construct/copy/destroy

    explicit assoc_vector(const key_compare& comp = key_compare(), const _A& alloc = _A())
		:	base(alloc), my_compare(comp)
    {}
    
    template<class _It>
    assoc_vector(_It first, _It last, 
		const key_compare& comp = key_compare(), 
        const _A& alloc = _A())
		:	base(first, last, alloc),
			my_compare(comp)
    {
        my_compare& me = *this;
        std::sort(begin(), end(), me);
    }
    
    assoc_vector& operator=(const assoc_vector& rhs)
    { 
        assoc_vector(rhs).swap(*this); 
        return *this;
    }
	
	void reserve(size_type count)
		{ base::reserve(count); }

    // iterators:
    // The following are here because MWCW gets 'using' wrong
    iterator begin() { return base::begin(); }
    const_iterator begin() const { return base::begin(); }
    iterator end() { return base::end(); }
    const_iterator end() const { return base::end(); }
    reverse_iterator rbegin() { return base::rbegin(); }
    const_reverse_iterator rbegin() const { return base::rbegin(); }
    reverse_iterator rend() { return base::rend(); }
    const_reverse_iterator rend() const { return base::rend(); }
    
    // capacity:
    bool empty() const { return base::empty(); }
    size_type size() const { return base::size(); }
    size_type max_size() { return base::max_size(); }

    // 23.3.1.2 element access:
    mapped_type& operator[](const key_type& key)
		{ return insert(value_type(key, mapped_type())).first->second; }

    // modifiers:
    std::pair<iterator, bool> insert(const value_type& val)
    {
        bool found(true);
        iterator i(lower_bound(val.first));

        if (i == end() || operator()(val.first, i->first))
        {
            i = base::insert(i, val);
            found = false;
        }
        return std::make_pair(i, !found);
    }

    iterator insert(iterator pos, const value_type& val)
    {
        if (pos != end() && operator()(*pos, val) && 
            (pos == end() - 1 ||
                !operator()(val, pos[1]) &&
                    operator()(pos[1], val)))
        {
            return base::insert(pos, val);
        }
        return insert(val).first;
    }
   
    template <class _It>
    void insert(_It first, _It last)
		{ for (; first != last; ++first) insert(*first); }
    
    void erase(iterator pos)
		{ base::erase(pos); }

    size_type erase(const key_type& k)
    {
        iterator i(find(k));
        if (i == end()) return 0;
        erase(i);
        return 1;
    }

    void erase(iterator first, iterator last)
		{ base::erase(first, last); }

    void swap(assoc_vector& other)
    {
        using std::swap;
        base::swap(other);
        my_compare& me = *this;
        my_compare& rhs = other;
        swap(me, rhs);
    }
    
    void clear()
		{ base::clear(); }

    // observers:
    key_compare key_comp() const
		{ return *this; }

    value_compare value_comp() const
    {
        const key_compare& comp = *this;
        return value_compare(comp);
    }

    // 23.3.1.3 map operations:
    iterator find(const key_type& k)
    {
        iterator i(lower_bound(k));
        if (i != end() && operator()(k, i->first))
        {
            i = end();
        }
        return i;
    }

    const_iterator find(const key_type& k) const
    {       
        const_iterator i(lower_bound(k));
        if (i != end() && operator()(k, i->first))
        {
            i = end();
        }
        return i;
    }

    size_type count(const key_type& k) const
		{ return find(k) != end(); }

    iterator lower_bound(const key_type& k)
    {
        my_compare& me = *this;
        return std::lower_bound(begin(), end(), k, me);
    }

    const_iterator lower_bound(const key_type& k) const
    {
        const my_compare& me = *this;
        return std::lower_bound(begin(), end(), k, me);
    }

    iterator upper_bound(const key_type& k)
    {
        my_compare& me = *this;
        return std::upper_bound(begin(), end(), k, me);
    }

    const_iterator upper_bound(const key_type& k) const
    {
        const my_compare& me = *this;
        return std::upper_bound(begin(), end(), k, me);
    }

    std::pair<iterator, iterator> equal_range(const key_type& k)
    {
        my_compare& me = *this;
        return std::equal_range(begin(), end(), k, me);
    }

    std::pair<const_iterator, const_iterator> equal_range(
        const key_type& k) const
    {
        const my_compare& me = *this;
        return std::equal_range(begin(), end(), k, me);
    }
    
    friend bool operator==(const assoc_vector& lhs, const assoc_vector& rhs)
    {
        const base& me = lhs;
        return me == rhs;
    } 

    bool operator<(const assoc_vector& rhs) const
    {
        const base& me = *this;
        const base& yo = rhs;
        return me < yo;
    } 

    friend bool operator!=(const assoc_vector& lhs, const assoc_vector& rhs)
		{ return !(lhs == rhs); } 

    friend bool operator>(const assoc_vector& lhs, const assoc_vector& rhs)
		{ return rhs < lhs; }

    friend bool operator>=(const assoc_vector& lhs, const assoc_vector& rhs)
		{ return !(lhs < rhs); } 

    friend bool operator<=(const assoc_vector& lhs, const assoc_vector& rhs)
		{ return !(rhs < lhs); }
};

// specialized algorithms:
template <class K, class V, class C, class A>
void swap(assoc_vector<K, V, C, A>& lhs, assoc_vector<K, V, C, A>& rhs)
	{ lhs.swap(rhs); }
    
} // namespace nucleon

////////////////////////////////////////////////////////////////////////////////
// Change log:
// May 20, 2001: change operator= - credit due to Cristoph Koegl
// June 11, 2001: remove paren in equal_range - credit due to Cristoph Koegl
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// January 22, 2002: fixed operator= - credit due to Tom Hyer
// June 25, 2002: fixed template insert() - credit due to Robert Minsk
// June 27, 2002: fixed member swap() - credit due to David Brookman
// April 27, 2005: adapted for the Nucleon Library, added reserve()
////////////////////////////////////////////////////////////////////////////////

#endif // _NUCLEON_ASSOC_VECTOR_H_
