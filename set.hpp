#pragma once

#include <functional>
#include "utility.hpp"
#include "iterator.hpp"
#include "rb_tree.hpp"

namespace ft
{
	template <class Key, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
	class set : public RBTree<Key, Node<Key>, RBIteratorConst<Key, Compare, Node<Key> >, RBIteratorConst<const Key, Compare, Node<Key> >, Compare, Alloc>
	{
		public:
			typedef Key														key_type;
			typedef Key														value_type;
			typedef Compare													key_compare;
			typedef typename Alloc::template rebind<Node<Key> >::other		allocator_type;
			typedef typename allocator_type::reference						reference;
			typedef typename allocator_type::const_reference				const_reference;
			typedef typename allocator_type::pointer						pointer;
			typedef typename allocator_type::const_pointer					const_pointer;
			typedef typename allocator_type::size_type						size_type;
			typedef RBIteratorConst<Key, Compare, Node<Key> >				iterator;
			typedef RBIteratorConst<const Key, Compare, Node<Key> >			const_iterator;

			// * COSTRUTTORI * //

			explicit set(const Compare& comp = Compare(), const Alloc& alloc = Alloc())
			{
				this->_key_compare = comp;
				this->_key_type = key_type();
				this->_value_type = value_type();
				(void)alloc;
			};

			template <class InputIt>
			set(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
			{
				this->_key_compare = comp;
				this->_key_type = key_type();
				this->_value_type = value_type();
				this->insert(first, last);
				(void)alloc;
			};

			set(const set& other)
			{
				this->_key_type = other._key_type;
				this->_value_type = other._value_type;
				this->_key_compare = other._key_compare;
				if (other.begin() != other.end())
					this->insert(other.begin(), other.end());
			};

			set&	operator=(const set& rhs)
			{
				if (this == &rhs)
					return (*this);
				this->_key_type = rhs._key_type;
				this->_value_type = rhs._value_type;
				this->_key_compare = rhs._key_compare;
				this->_alloc = rhs._alloc;
				this->clear();
				this->insert(rhs.begin(), rhs.end());
				return (*this);
			};

			~set()
			{
				clear();
			};

			// * MEMBER FUNCTION *//

			ft::pair<iterator, bool> insert(Key const &value)
			{
				ft::pair<iterator, bool>	dst;
				pointer node = this->_alloc.allocate(1);

				node->color = RED;
				node->parent = this->_sentinel;
				node->child[LEFT] = this->_sentinel;
				node->child[RIGHT] = this->_sentinel;
				node->data = value;

				if (this->empty())
				{
					this->_root = node;
					this->_sentinel->parent = node;
					node->color = BLACK;
					this->_size++;
					dst.first = iterator(node, this->_sentinel);
					dst.second = true;
					return (dst);
				}
				else
				{
					dst.first = iterator(node, this->_sentinel);
					dst.second = false;
					if (this->_c(value, this->_root->data) && this->_root->data != value)
						return (insertNode(this->_root->child[LEFT], node, this->_root, 1));
					else if (this->_c(this->_root->data, value) && this->_root->data != value)
						return (insertNode(this->_root->child[RIGHT], node, this->_root, 1));
					else
					{
						dst.first = iterator(node, this->_sentinel);
						dst.second = false;
						this->_alloc.deallocate(node, 1);
						return (dst);
					}
				}
			};

			template <class InputIt>
			void	insert(InputIt first, InputIt last)
			{
				while (first != last)
					this->insert(*first++);
			};

			iterator	insert(iterator pos, const Key & val)
			{
				ft::pair<iterator, bool> dst;

				(void)pos;
				dst = this->insert(val);
				return (dst.first);
			};

			ft::pair<iterator, bool> insertNode(pointer &start, pointer &node, pointer& parent, int flag)
			{
				ft::pair<iterator, bool>	dst;

				if (!start || start == this->_sentinel)
				{
					node->parent = parent;
					if (this->_c(node->data, parent->data))
						parent->child[LEFT] = node;
					else
						parent->child[RIGHT] = node;
					start = node;
					if (flag)
						this->_size++;
					this->balanceInsert(start);
					dst.first = iterator(node, this->_sentinel);
					dst.second = true;
					return (dst);
				}
				if (this->_c(node->data, start->data))
					return (insertNode(start->child[LEFT], node, start, flag));
				else if (this->_c(start->data, node->data))
					return (insertNode(start->child[RIGHT], node, start, flag));
				dst.first = iterator(node, this->_sentinel);
				this->_alloc.deallocate(node, 1);
				dst.second = false;
				return (dst);
			};

			//------------------------------------------------------//

			iterator	erase(iterator pos)
			{
				iterator	ret = this->getSuccessor(pos.node);

				this->erase_deep(*pos);
				return (ret);
			}

			iterator	erase(iterator first, iterator last)
			{
				while (first != last)
					this->erase_deep(*first++);
				return (last.node);
			}

			size_type	erase(const Key& key)
			{
				if (erase_deep(key) != iterator(NULL, this->_sentinel))
					return (1);
				return (0);
			}

			iterator	erase_deep(Key const & val)
			{
				pointer		node = this->find(val).node;
				pointer		tmp;
				pointer		successor;
				iterator	ret;

				if (node && node != this->_sentinel)
					ret = this->find(this->getSuccessor(node)->data);
				else
					return (iterator(NULL, this->_sentinel));

				if ((!node->child[LEFT] && !node->child[RIGHT]) || (node->child[LEFT] == this->_sentinel && node->child[RIGHT] == this->_sentinel))
				{
					tmp = node->parent;
					if (node->color == BLACK && node != this->_root)
						this->balanceDelete(node);
					if (tmp != this->_sentinel)
						this->unlink(tmp, node);
					else
					{
						this->_root = this->_sentinel;
						this->_sentinel->parent = this->_root;
					}
				}
				else if (this->oneChild(node) != this->_sentinel)
				{
					if (node->color == BLACK && node != this->_root)
					{
						if (this->oneChild(node)->color != RED)
							this->balanceDelete(node);
						else
							this->oneChild(node)->color = BLACK;
					}
					this->oneChild(node)->parent = node->parent;
					if (node != this->_root)
						this->link(node->parent, node, this->oneChild(node));
					else
					{
						this->_root = this->oneChild(node);
						this->_sentinel->parent = this->_root;
					}
				}
				else
				{
					if (this->_c(node->data, this->_root->data))
						successor = this->getSuccessor(node);
					else
						successor = this->getPredecessor(node);
					this->balanceDelete(successor);
					if (successor->child[RIGHT] != this->_sentinel)
					{
						successor->child[RIGHT] = this->_sentinel;
					}
					if (node != this->_root)
					{
						this->unlink(successor->parent, successor);
						this->link(node->parent, node, successor);
						if (node->child[LEFT] != this->_sentinel)
						{
							node->child[LEFT]->parent = successor;
							successor->child[LEFT] = node->child[LEFT];
						}
						if (node->child[RIGHT] != this->_sentinel)
						{
							if (successor->child[RIGHT] != this->_sentinel)
								this->link(successor->parent, successor, successor->child[RIGHT]);
							node->child[RIGHT]->parent = successor;
							successor->child[RIGHT] = node->child[RIGHT];
						}
					}
					else
					{
						pointer toHandle = NULL;
						pointer	toHandle2 = NULL;
						this->_root = successor;
						this->_sentinel->parent = successor;
						if (this->_root->parent->child[LEFT] == this->_root)
							this->_root->parent->child[LEFT] = this->_sentinel;
						else
							this->_root->parent->child[RIGHT] = this->_sentinel;
						this->_root->parent = this->_sentinel;
						if (node->child[RIGHT] == successor)
						{
							if (node->child[RIGHT] != this->_sentinel)
								toHandle = node->child[RIGHT];
							this->_root->child[LEFT] = node->child[LEFT];
							node->child[LEFT]->parent = this->_root;
							this->_root->color = BLACK;
						}
						else
						{
							if (node->child[LEFT] != this->_sentinel)
								toHandle2 = node->child[LEFT];
							this->_root->child[RIGHT] = node->child[RIGHT];
							node->child[RIGHT]->parent = this->_root;
							this->_root->color = BLACK;
						}
						if (toHandle)
							insertNode(this->_root, toHandle, this->_root, 0);
						if (toHandle2)
							insertNode(this->_root, toHandle2, this->_root, 0);
					}

				}
				this->_alloc.deallocate(node, 1);
				node = NULL;
				this->_size--;
				return (ret);
			}

			//------------------------------------------------------//

			void	clear()
			{
				while (this->_size)
					this->erase(this->min());
			};

			//------------------------------------------------------//

			class value_compare : public std::binary_function<value_type, value_type, bool>
			{
				friend class set<Key>;

				private:
					Compare comp;
					value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
				public:
					bool operator() (const value_type& x, const value_type& y) const
					{
						return comp(x, y);
					}
			};

			value_compare	value_comp() const
			{
				return (value_compare(this->key_comp()));
			};

			//------------------------------------------------------//

			iterator	find(Key const & val) const
			{
				pointer	node = this->_root;

				if (!node || node == this->_sentinel)
					return (iterator(this->_sentinel, this->_sentinel));

				return (findPointer(node, val));
			};

			iterator	findPointer(pointer& start, Key const & val) const
			{
				if (!start || start->color == SENTINEL)		// valore inesistente
					return (iterator(this->_sentinel, this->_sentinel));

				if (start->data == val)			// valore uguale
					return (iterator(start, this->_sentinel));
				if (this->_c(val, start->data))
					return (findPointer(start->child[LEFT], val));
				if (this->_c(start->data, val))	// valore maggiore
					return (findPointer(start->child[RIGHT], val));
				else
					return (iterator(start, this->_sentinel));
			};

			//------------------------------------------------------//

			size_type	count(Key const & key) const
			{
				iterator	dst = find(key);
				if (dst == iterator(this->_sentinel, this->_sentinel))
					return (0);
				return (1);
			};

			//------------------------------------------------------//

			iterator	lower_bound(Key const & key)
			{
				iterator	tmp = this->begin();

				while (tmp != this->end() && this->_c(tmp.node->data, key))
					tmp++;
				return (tmp);
			};

			const_iterator	lower_bound(Key const & key) const
			{
				const_iterator	tmp = this->begin();

				while (tmp != this->end() && this->_c(tmp.node->data, key))
					tmp++;
				return (tmp);
			};

			iterator	upper_bound(Key const & key)
			{
				iterator	tmp = this->begin();

				while (tmp != this->end() && !this->_c(key, tmp.node->data))
					tmp++;
				return (tmp);
			};

			const_iterator	upper_bound(Key const & key) const
			{
				const_iterator	tmp = this->begin();

				while (tmp != this->end() && !this->_c(key, tmp.node->data))
					tmp++;
				return (tmp);
			};

			ft::pair<iterator, iterator>	equal_range(const Key& key)
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			ft::pair<const_iterator, const_iterator>	equal_range(const Key& key) const
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

	};

	template <class T, class Compare, class Alloc>
	bool operator==(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return ((lhs.size() == rhs.size()) && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	};

	template <class T, class Compare, class Alloc>
	bool operator!=(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return (!(lhs == rhs));
	};

	template <class T, class Compare, class Alloc>
	bool operator<(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	};

	template <class T, class Compare, class Alloc>
	bool operator<=(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return (!(lhs > rhs));
	};

	template <class T, class Compare, class Alloc>
	bool operator>(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return (!(lhs == rhs || lhs < rhs));
	};

	template <class T, class Compare, class Alloc>
	bool operator>=(set<T, Compare, Alloc> const & lhs, set<T, Compare, Alloc> const & rhs)
	{
		return (!(lhs < rhs));
	};

}
