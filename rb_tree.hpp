/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rb_tree.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 13:53:02 by ddelladi          #+#    #+#             */
/*   Updated: 2022/12/05 18:23:00 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <memory>
#include <utility>
#include <iostream>
#include <limits.h>
#include "utility.hpp"
#include "iterator.hpp"

namespace ft
{
	enum node_color
	{
		RED,
		BLACK,
		SENTINEL
	};

	enum branch
	{
		LEFT,
		RIGHT
	};

	template <typename T>
	struct Node
	{
		node_color	color;
		Node		*parent;
		Node		*child[2];
		T 			data;

		template <class U, class V>
		Node(ft::pair<U, V> const & val) : data(val) {};
	};

	template <class Pair>
	struct NodeRB2 {
		NodeRB2					*parent;
		NodeRB2 				*left;
		NodeRB2 				*right;
		Pair					data;
		int						color;
	};

	template <class Key, class NodeType, class Iterator, class ConstIterator, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
	class RBTree
	{

	public:
		typedef NodeRB2<Key>											originalNode;
		typedef Key														key_type;
		typedef Key														value_type;
		typedef Compare													key_compare;
		typedef Compare													value_compare;
		typedef typename Alloc::template rebind<NodeType>::other		allocator_type;
		typedef typename Alloc::template rebind<originalNode>::other	allocator_type2;
		typedef typename allocator_type::reference						reference;
		typedef typename allocator_type::const_reference				const_reference;
		typedef typename allocator_type::pointer						pointer;
		typedef typename allocator_type::const_pointer					const_pointer;
		typedef typename allocator_type::size_type						size_type;
		typedef Iterator												iterator;
		typedef ConstIterator											const_iterator;
		typedef typename ft::reverse_iterator<iterator>					reverse_iterator;
		typedef typename ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
		
		RBTree() :	_root(NULL),
						_size(0),
						_alloc(allocator_type())
		{
			_sentinel = _alloc.allocate(1);
			_sentinel->color = SENTINEL;
			_root = _sentinel;
			_sentinel->parent = _root;
		};

		RBTree(RBTree const &src)
		{
			_alloc = allocator_type();
			_sentinel = _alloc.allocate(1);
			_sentinel->color = SENTINEL;
			_root = _sentinel;
			_sentinel->parent = _root;
			_root = _sentinel;
			_size = 0;
			iterator	iter = src.begin();
			while (iter != src.end())
				this->insert(*iter++);
		};

		RBTree &operator=(RBTree const &rhs)
		{
			if (this == &rhs)
				return (*this);
			_alloc = rhs.get_allocator();
			_sentinel = _alloc.allocate(1);
			_sentinel->color = SENTINEL;
			_root = _sentinel;
			_sentinel->parent = _root;
			_root = _sentinel;
			_size = 0;
			this->insert(rhs.begin(), rhs.end());
			return (*this);
		};

		~RBTree() { _alloc.deallocate(_sentinel, 1); };

		allocator_type	get_allocator() const { return (this->_alloc); }

		bool empty() const { return ((!this->_size) ? true : false); }
		size_type size() const { return this->_size; }
		size_type max_size() const { return _alloc2.max_size(); }

		

		iterator				begin() { return (iterator(min(), _sentinel)); }
		const_iterator			begin() const { return (const_iterator(min(), _sentinel)); }
		iterator				end() { return iterator(_sentinel, _sentinel); }
		const_iterator			end() const { return const_iterator(_sentinel, _sentinel); }
		reverse_iterator		rbegin() { return (reverse_iterator(end())); }
		const_reverse_iterator	rbegin() const { return (const_iterator(end())); }
		reverse_iterator		rend() { return (reverse_iterator(begin())); }
		const_reverse_iterator	rend() const { return (const_reverse_iterator(begin())); }


		virtual iterator					findPointer(pointer& start, Key const & val) const = 0;
		virtual iterator					erase_deep(Key const & val) = 0;
		virtual ft::pair<iterator, bool>	insert(Key const &val) = 0;
		virtual ft::pair<iterator, bool>	insertNode(pointer &start, pointer &node, pointer& parent, int flag) = 0;
		virtual void						clear() = 0;

		pointer	getPredecessor(pointer const & node) const
		{
			pointer	tmp = node;
			
			if (node == min())
				return (_sentinel);
			if (tmp->child[LEFT] != _sentinel)
				return (max(tmp->child[LEFT]));
			else
			{
				while (tmp->parent != _sentinel)
				{
					if (tmp->parent->child[RIGHT] == tmp)
					{
						tmp = tmp->parent;
						break ;
					}
					tmp = tmp->parent;
				}
				return (tmp);
			}
		}

		pointer	getSuccessor(pointer const & node) const
		{
			pointer	tmp = node;
			
			if (node == max())
				return (_sentinel);
			if (tmp->child[RIGHT] != _sentinel)
				return (min(tmp->child[RIGHT]));
			else
			{
				while (tmp->parent != _sentinel)
				{
					if (tmp->parent->child[LEFT] == tmp)
					{
						tmp = tmp->parent;
						break ;
					}
					tmp = tmp->parent;
				}
				return (tmp);
			}
		}

		pointer	min() const
		{
			const pointer*	node = &_root;

			if (!(*node) || (*node) == _sentinel)
				return (_sentinel);

			while ((*node)->child[LEFT] && (*node)->child[LEFT] != _sentinel)
				node = &(*node)->child[LEFT];
			return (*node);
		}

		pointer	min(pointer const & node) const
		{
			const pointer*	tmp = &node;

			if (!(*tmp) || (*tmp) == _sentinel)
				return (_sentinel);

			while ((*tmp)->child[LEFT] && (*tmp)->child[LEFT] != _sentinel)
				tmp = &(*tmp)->child[LEFT];
			return (*tmp);
		}
		
		pointer	max() const
		{
			pointer	node = _root;

			if (!node || node == _sentinel)
				return (_sentinel);

			while (node->child[RIGHT] && node->child[RIGHT] != _sentinel)
				node = node->child[RIGHT];
			return (node);
		}

		pointer	max(pointer const & node) const
		{
			pointer	tmp = node;

			if (!node || node == _sentinel)
				return (_sentinel);

			while (tmp->child[RIGHT] != _sentinel)
				tmp = tmp->child[RIGHT];
			return (tmp);
		}

		void			swap(RBTree & rhs)
		{
			if (this == &rhs)
				return ;
			
			pointer			tmpRoot = this->_root;
			pointer			tmpSentinel = this->_sentinel;
			size_type		tmpSize = this->_size;
			allocator_type	tmpAllocatorType = this->_alloc;
			Compare			tmpCompare = this->_c;

			this->_root = rhs._root;
			this->_sentinel = rhs._sentinel;
			this->_size = rhs._size;
			this->_alloc = rhs._alloc;
			this->_c = rhs._c;

			rhs._root = tmpRoot;
			rhs._sentinel = tmpSentinel;
			rhs._size = tmpSize;
			rhs._alloc = tmpAllocatorType;
			rhs._c = tmpCompare;
		}

		key_compare		key_comp() const { return (this->_key_compare); }

	protected:
		key_type		_key_type;
		value_type		_value_type;
		key_compare		_key_compare;
		value_compare	_value_compare;
		pointer			_root;
		pointer			_sentinel;
		size_type		_size;
		allocator_type	_alloc;
		allocator_type2	_alloc2;
		Compare			_c;

		pointer	getOnlyChildWithoutChildren(pointer const & node)
		{
			pointer	child;

			if (node->child[LEFT] != _sentinel && node->child[RIGHT] == _sentinel)
				child = node->child[LEFT];
			else if (node->child[LEFT] == _sentinel && node->child[RIGHT] != _sentinel)
				child = node->child[RIGHT];
			else
				return (NULL);
			if (child->child[LEFT] != _sentinel || child->child[RIGHT] != _sentinel)
				return (NULL);
			return (child);
		}

		pointer*	rotateLeft(pointer & node)
		{
			pointer		toHandle;
			pointer*	tmp = &node;
			pointer*	ret;

			if ((*tmp)->child[RIGHT]->child[LEFT] != _sentinel)
				toHandle = (*tmp)->child[RIGHT]->child[LEFT];
			else
				toHandle = NULL;
			if ((*tmp) == _root)
			{
				_root = (*tmp)->child[RIGHT];
				_sentinel->parent = _root;
				(*tmp)->child[RIGHT]->parent = _sentinel;
			}
			else
			{
				(*tmp)->child[RIGHT]->parent = (*tmp)->parent;
				if ((*tmp)->parent->child[RIGHT] == *tmp)
					(*tmp)->parent->child[RIGHT] = (*tmp)->child[RIGHT];
				else
					(*tmp)->parent->child[LEFT] = (*tmp)->child[RIGHT];
			}
			(*tmp)->parent = (*tmp)->child[RIGHT];
			(*tmp)->child[RIGHT]->child[LEFT] = (*tmp);
			(*tmp)->child[RIGHT] = _sentinel;
			if (toHandle)
				insertNode((*tmp)->parent, toHandle, (*tmp)->parent, 0);
			ret = &(*tmp)->child[LEFT];
			return (ret);
		}

		pointer*	rotateRight(pointer & node)
		{
			pointer		toHandle;
			pointer*	tmp = &node;
			pointer*	ret;

			if ((*tmp) && (*tmp)->child[LEFT] != _sentinel && (*tmp)->child[LEFT]->child[RIGHT] != _sentinel)
				toHandle = (*tmp)->child[LEFT]->child[RIGHT];
			else
				toHandle = NULL;
			if ((*tmp) == _root)
			{
				_root = (*tmp)->child[LEFT];
				_sentinel->parent = _root;
				(*tmp)->child[LEFT]->parent = _sentinel;
			}
			else
			{
				(*tmp)->child[LEFT]->parent = (*tmp)->parent;
				if ((*tmp)->parent->child[RIGHT] == *tmp)
					(*tmp)->parent->child[RIGHT] = (*tmp)->child[LEFT];
				else
					(*tmp)->parent->child[LEFT] = (*tmp)->child[LEFT];
			}
			(*tmp)->parent = (*tmp)->child[LEFT];
			(*tmp)->child[LEFT]->child[RIGHT] = (*tmp);
			(*tmp)->child[LEFT] = _sentinel;
			if (toHandle)
				insertNode((*tmp), toHandle, (*tmp), 0);
			ret = &(*tmp)->child[RIGHT];
			return (ret);
		}

		void	balanceDelete(pointer & node)
		{
			pointer*	tmp = &node;
			pointer		sibling;
			pointer		leftNephew;
			pointer		rightNephew;

			while ((*tmp) != _root && (*tmp)->color == BLACK)
			{
				getRelatives2((*tmp), sibling, leftNephew, rightNephew);
				if ((*tmp) == (*tmp)->parent->child[LEFT])
				{
					if (sibling && sibling->color == RED)
					{
						(*tmp)->parent->color = RED;
						sibling->color = BLACK;
						tmp = rotateLeft((*tmp)->parent);
					}
					else
					{
						if (leftNephew && leftNephew->color == BLACK && rightNephew && rightNephew->color == BLACK)
						{
							sibling->color = RED;
							tmp = &(*tmp)->parent;
						}
						else if (leftNephew && leftNephew->color == RED && rightNephew && rightNephew->color == BLACK)
						{
							leftNephew->color = BLACK;
							sibling->color = RED;
							tmp = rotateRight(sibling);
						}
						else if (rightNephew && rightNephew->color == RED)
						{
							sibling->color = (*tmp)->parent->color;
							(*tmp)->parent->color = BLACK;
							rightNephew->color = BLACK;
							rotateLeft((*tmp)->parent);
							break ;
						}
						else if (oneChild(*tmp) && oneChild(*tmp)->color == RED)
						{
							oneChild(*tmp)->color = BLACK;
							break ;
						}
						else if (sibling && sibling->color == BLACK && (*tmp)->color == BLACK)
						{
							sibling->color = RED;
							(*tmp)->parent->color = BLACK;
							tmp = &(*tmp)->parent;
							break ;
						}
						else
							tmp = &(*tmp)->parent;
					}
				}
				else
				{
					if (sibling && sibling->color == RED)
					{
						(*tmp)->parent->color = RED;
						sibling->color = BLACK;
						tmp = rotateRight((*tmp)->parent);
					}
					else
					{
						if (rightNephew && rightNephew->color == BLACK && leftNephew&& leftNephew->color == BLACK)
						{
							sibling->color = RED;
							tmp = &(*tmp)->parent;
						}
						else if (rightNephew && rightNephew->color == RED && leftNephew && leftNephew->color == BLACK)
						{
							rightNephew->color = BLACK;
							sibling->color = RED;
							tmp = rotateLeft(sibling);
						}
						else if (leftNephew && leftNephew->color == RED)
						{
							sibling->color = (*tmp)->parent->color;
							(*tmp)->parent->color = BLACK;
							leftNephew->color = BLACK;
							rotateRight((*tmp)->parent);
							break ;
						}
						else if (oneChild(*tmp)->color == RED)
						{
							oneChild(*tmp)->color = BLACK;
							break ;
						}
						else if (sibling && sibling->color == BLACK && (*tmp)->color == BLACK)
						{
							sibling->color = RED;
							(*tmp)->parent->color = BLACK;
							tmp = &(*tmp)->parent;
							break ;
						}
						else
							tmp = &(*tmp)->parent;
					}
				}
			}
		}

		void	getRelatives(pointer & parent, pointer & grandParent, pointer & uncle)
		{
			if (parent != _sentinel)
				grandParent = parent->parent;
			else
				grandParent = NULL;
			if (grandParent != _sentinel && grandParent)
			{
				if (grandParent->child[LEFT] == parent && grandParent->child[RIGHT] != _sentinel)
					uncle = grandParent->child[RIGHT];
				else if (grandParent->child[RIGHT] == parent && grandParent->child[LEFT] != _sentinel)
					uncle = grandParent->child[LEFT];
				else
					uncle = NULL;
			}
			else
				uncle = NULL;
		}

		int	hasUncle(pointer & node)
		{
			if (node->parent == _sentinel)
				return (0);
			if (node->parent->child[LEFT] != _sentinel && node->parent->child[LEFT] == node && node->parent->child[RIGHT] != _sentinel)
				return (1);
			if (node->parent->child[RIGHT] != _sentinel && node->parent->child[RIGHT] == node && node->parent->child[LEFT] != _sentinel)
				return (1);
			return (0);
		}

		pointer&	getUncle(pointer & node)
		{
			if (node->parent->child[LEFT] == node)
				return (node->parent->child[RIGHT]);
			return (node->parent->child[LEFT]);
		}

		void	balanceInsert(pointer & node)
		{
			pointer*	tmp = &node;
			pointer		parent = node->parent;
			pointer		grandParent;
			pointer		uncle;

			while (1 && *tmp && *tmp != _sentinel)
			{
				parent = (*tmp)->parent;
				getRelatives(parent, grandParent, uncle);
				if (!parent || parent == _sentinel)
				{
					(*tmp)->color = BLACK;
					break ;
				}
				else if (parent->color == BLACK)
					break ;
				else if (uncle && (*tmp)->color == RED && parent->color == RED && uncle->color == RED)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					grandParent->color = RED;
					tmp = &(*tmp)->parent->parent;
				}
				else if ((*tmp)->color == RED && parent->color == RED)
				{
					if (parent->child[RIGHT] == (*tmp) && grandParent->child[LEFT] == parent)
						tmp = rotateLeft(parent);
					else if (parent->child[LEFT] == (*tmp) && grandParent->child[RIGHT] == parent)
						tmp = rotateRight(parent);
					else if (parent->child[LEFT] == (*tmp) && grandParent->child[LEFT] == parent)
					{
						parent->color = BLACK;
						grandParent->color = RED;
						rotateRight(grandParent);
						break ;
					}
					else if (parent->child[RIGHT] == (*tmp) && grandParent->child[RIGHT] == parent)
					{
						parent->color = BLACK;
						grandParent->color = RED;
						rotateLeft(grandParent);
						break ;
					}
				}
				else
					break ;
			}
		}

		void	getRelatives2(pointer & node, pointer & sibling, pointer & leftNephew, pointer & rightNephew)
		{
			if (node->parent->child[LEFT] != _sentinel && node->parent->color != SENTINEL && node->parent->child[LEFT] == node && node->parent->child[RIGHT] && node->parent->child[RIGHT] != _sentinel)
				sibling = node->parent->child[RIGHT];
			else if (node->parent->child[RIGHT] != _sentinel && node->parent->color != SENTINEL && node->parent->child[LEFT] && node->parent->child[LEFT] != _sentinel)
				sibling = node->parent->child[LEFT];
			else
			{
				sibling = NULL;
				leftNephew = NULL;
				rightNephew = NULL;
				return ;
			}
			if (sibling && sibling->child[LEFT] && sibling->child[LEFT] != _sentinel)
				leftNephew = sibling->child[LEFT];
			else
				leftNephew = NULL;
			if (sibling && sibling->child[RIGHT] && sibling->child[RIGHT] != _sentinel)
				rightNephew = sibling->child[RIGHT];
			else
				rightNephew = NULL;
		}

		pointer	getSibling(pointer & node)
		{
			if (node->parent == _sentinel)
				return (NULL);
			if (node->parent->child[LEFT] != _sentinel && node->parent->child[LEFT] == node)
				return (node->parent->child[RIGHT]);
			else if (node->parent->child[RIGHT] != _sentinel && node->parent->child[RIGHT] == node)
				return (node->parent->child[LEFT]);
			return (NULL);
		}

		pointer&	oneChild(pointer& node)
		{
			if (node->child[LEFT] != _sentinel && node->child[RIGHT] == _sentinel)
				return (node->child[LEFT]);
			else if (node->child[RIGHT] != _sentinel && node->child[LEFT] == _sentinel)
				return (node->child[RIGHT]);
			return (_sentinel);
		}

		void	link(pointer& parent, pointer& oldSon, pointer& node)
		{
			if (parent == _sentinel)
				parent->parent = node;
			else if (parent->child[LEFT] == oldSon)
				parent->child[LEFT] = node;
			else
				parent->child[RIGHT] = node;
			if (node != _sentinel)
				node->parent = parent;
		}

		void	unlink(pointer& parent, pointer& node)
		{
			if (parent->child[LEFT] == node)
			{
				parent->child[LEFT] = _sentinel;
				if (node->child[LEFT] != _sentinel && node->child[RIGHT] == _sentinel)
				{
					parent->child[LEFT] = node->child[LEFT];
					node->child[LEFT]->parent = parent;
				}
			}
			else
			{
				parent->child[RIGHT] = _sentinel;
				if (node->child[RIGHT] != _sentinel && node->child[LEFT] == _sentinel)
				{
					parent->child[RIGHT] = node->child[RIGHT];
					node->child[RIGHT]->parent = parent;
				}
			}
			node->parent = _sentinel;
		}
	};
}
