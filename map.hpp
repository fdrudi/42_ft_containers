#pragma once

#include <algorithm>
#include "utility.hpp"
#include "rb_tree.hpp"
#include "iterator.hpp"

namespace ft
{
	template<	class Key,
				class T,
				class Compare = std::less<Key>,
				class Allocator = std::allocator<std::pair<const Key, T> > >
	class map : public RBTree<	ft::pair<const Key, T>,
								Node<ft::pair<const Key, T> >,
								RBIterator<ft::pair<const Key, T>, Compare, Node<ft::pair<const Key, T> > >,
								RBIteratorConst<ft::pair<const Key, T>, Compare, Node<ft::pair<const Key, T> > >,
								Compare,
								Allocator >
	{
		public:

			typedef	Key																	key_type;
			typedef	T																	mapped_type;
			typedef	pair<const key_type, mapped_type>									value_type;
			typedef typename	Allocator::template rebind<Node<value_type> >::other	allocator_type;
			typedef RBIterator<value_type, Compare, Node<value_type> >					iterator;
			typedef RBIteratorConst<value_type, Compare, Node<value_type> >				const_iterator;

	};
}
