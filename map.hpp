#pragma once

#include <algorithm>
#include "utility.hpp"
#include "rb_tree.hpp"
#include "iterator.hpp"

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map : public RBTree<ft::pair<const Key, T>, Node<ft::pair<const Key, T> >,  RBIterator<ft::pair<const Key, T>, Compare, Node<ft::pair<const Key, T> > >, RBIteratorConst<ft::pair<const Key, T>, Compare, Node<ft::pair<const Key, T> > >, Compare, Allocator>
	{
		public:
			typedef Key																	key_type;
			typedef	T																	mapped_type;
			typedef ft::pair<const Key, T>												value_type;
			typedef typename ft::pair<const Key, T>										Pair;
			typedef typename ft::NodeRB2<Pair>											originalNode;
			typedef typename Allocator::template rebind<Node<value_type> >::other		allocator_type;
			typedef typename allocator_type::reference									reference;
			typedef typename allocator_type::const_reference							const_reference;
			typedef typename allocator_type::pointer									pointer;
			typedef typename allocator_type::const_pointer								const_pointer;
			typedef typename allocator_type::size_type									size_type;
			typedef RBIterator<value_type, Compare, Node<value_type> >					iterator;
			typedef RBIteratorConst<value_type, Compare, Node<value_type> >				const_iterator;
			typedef Compare																key_compare;


		// * COSTRUTTORI * //

			map() {};

		// Default Constructor (empty)
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
		{

		};

		// Range Constructor (da 'fisrt' a 'last')
		template <class InputIt>
		map (InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
		{

		};

		// Copy Constructor
		map (const map& x)
		{

		};

		// Copy Assign Operator
		map& operator=( const map& other )
		{

		};

		// Destructor
		~map()
		{

		};


		// * MEMBER FUNCTION *//

		mapped_type& operator[] (const key_type& k);
		mapped_type& at (const key_type& k);
		const mapped_type& at (const key_type& k) const;

		std::pair<iterator, bool> insert( const value_type& value );
		template< class InputIt >
		void insert( InputIt first, InputIt last );
		//?iterator insert (iterator position, const value_type& val); Da Fare ??

		iterator erase( iterator pos );
		iterator erase( iterator first, iterator last );
		size_type erase( const key_type& key );

		void clear();

		value_compare value_comp() const;

		iterator find (const key_type& k);
		const_iterator find (const key_type& k) const;

		size_type count (const key_type& k) const;

		iterator lower_bound (const key_type& k);
		const_iterator lower_bound (const key_type& k) const;

		iterator upper_bound (const key_type& k);
		const_iterator upper_bound (const key_type& k) const;

		pair<iterator,iterator> equal_range (const key_type& k);
		pair<const_iterator,const_iterator> equal_range (const key_type& k) const;




		//! IMPLEMETATE IN RBTREE
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;

		bool empty() const;
		size_type size() const;
		size_type max_size() const;

		void swap (map& other);

		key_compare key_comp() const;

		allocator_type get_allocator() const;




	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator==( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator!=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator<( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )

	template< class Key, class T, class Compare, class Alloc >
	bool operator<=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator>( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator>=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

}
