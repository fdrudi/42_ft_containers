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

			typedef	Key																	key_type;
			typedef	T																	mapped_type;
			typedef	pair<const key_type, mapped_type>									value_type;
			typedef typename	Allocator::template rebind<Node<value_type> >::other	allocator_type;
			typedef RBIterator<value_type, Compare, Node<value_type> >					iterator;
			typedef RBIteratorConst<value_type, Compare, Node<value_type> >				const_iterator;


			// * COSTRUTTORI * //

			map() {};

			// Default Constructor (empty)
			explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			{
				(void)comp;
				(void)alloc;
			};

			// Range Constructor (da 'fisrt' a 'last')
			template <class InputIt>
			map (InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			{
				(void)comp;
				(void)alloc;
				this->insert(first, last);
			};

			// Copy Constructor
			map (const map& other)
			{
				this->insert(other.begin(), other.end());
			};

			// Copy Assign Operator
			map& operator=( const map& other )
			{
				if (this == &other)
					return (*this);
				this->clear();
				this->insert(other.begin(), other.end());
				return (*this);

			};

			// Destructor
			~map()
			{
				this->clear();
			};


			// * MEMBER FUNCTION *//


			mapped_type& operator[] (const key_type& k)
			{
				try
				{
					return at(key);
				}
				catch(const std::exception& e)
				{
					ft::pair<key_type, mapped_type>	ret(key, mapped_type());
					insert(ret);
					return (at(key));
				}
			};

			mapped_type& at (const key_type& k)
			{
				iterator tmp = find(k);

				if (tmp.node == this->_sentinel)
					throw std::out_of_range("ft::map::at");
				return (tmp.second);
			};

			const mapped_type& at (const key_type& k) const
			{
				return ((const) at(k));
			};

			std::pair<iterator, bool> insert( const value_type& value )
			{

			};

			template< class InputIt >
			void insert( InputIt first, InputIt last )
			{

			};

			//!iterator insert (iterator position, const value_type& val);

			/*	Funzione helper ricorsiva che serve ad inserire il nodo controllando le dipendenze con il parent
				che viene modificato finchè non arriva a nullo o a sentinel, andando poi ad inserire il nodo effettivamente.
				Se la Key passata è già presente nella mappa, la funzione elimina il nodo e torna false nel booleano */
			ft::pair<iterator, bool>	insertNode(pointer &start, pointer &node, pointer& parent, int flag)
			{
				ft::pair<iterator, bool>	dst;

				if (!start || start = this->_sentinel)
				{
					node->parent = parent;
					if (this->_c(node->data.first, parent->data.first))
						parent->child[LEFT] = node;
					else
						parent->child[RIGHT] = node;
					start = node;
					if (flag)
						this->_size++;
					this->balanceInsert(start);
					dst.first = iterator(node, this->_sentinel);
					dst.first = false;
				}
				if (this->_c(node->data.fisrt, start->data.fisrt))
					return (insertNode(start->child[LEFT], node, start, flag));
				else if (this->_c(start->data.first, node->data.first))
					return (insertNode(start->child[RIGHT], node, start, flag));
				dst.first = find(node->data.first);
				delete node;
				dst.second = false;
				return (dst);
			};

			iterator erase(iterator pos)
			{

			};

			iterator erase(iterator first, iterator last)
			{

			};

			size_type erase( const key_type& key )
			{

			};

			/* Questa funzione rimuove un elemento dalla mappa utilizzando una ricerca in profondità.
			   In generale, la funzione cerca l'elemento con la chiave specificata e poi decide come rimuoverlo
			   in base alla sua posizione all'interno dell'albero rosso-nero.
			   Se l'elemento da rimuovere è un nodo foglia, il nodo viene semplicemente rimosso dall'albero e
			   viene ripristinata la proprietà di bilanciamento dell'albero rosso-nero.
			   Se l'elemento da rimuovere ha un solo figlio, il figlio prende il posto del nodo e la proprietà di bilanciamento dell'albero viene ripristinata.
			   Se l'elemento da rimuovere ha due figli, viene selezionato il successore o il predecessore del nodo, che diventa il nuovo nodo da rimuovere.
			   Il successore/predecessore viene quindi rimosso come sopra, e il nodo originale viene sostituito dal successore/predecessore.
			   In tutti i casi, il nodo viene deallocato e la dimensione della mappa viene ridotta.
			   La funzione restituisce un iteratore al successore dell'elemento rimosso. */
			iterator erase_deep(Key const & val)
			{

			};

			void clear();

			value_compare value_comp() const;

			iterator find (const key_type& k)
			{
				pointer							node = this->_root;
				ft::pair<key_type, mapped_type>	ret(key, mapped_type());

				if (!node || node == this->_sentinel)
					return (iterator(_sentinel, _sentinel));

				return (findPointer(node, ret));
			};

			const_iterator find (const key_type& k) const
			{
				pointer							node = this->_root;
				ft::pair<key_type, mapped_type>	ret(key, mapped_type());

				if (!node || node == this->_sentinel)
					return (iterator(_sentinel, _sentinel));

				return (findPointer(node, ret));
			};

			iterator	findPointer(pointer& start, Key const & val) const
			{
				if (!start || start.color == SENTINEL)		// valore inesistente
					return (iterator(_sentinel, _sentinel));
				if (start->data.first == val.first)			// valore uguale
					return (iterator(start, _sentinel));
				if (this->_c(start->data.first, val.first))	// valore maggiore
					return (findPointer(start->child[RIGHT], val));
				return (findPointer(start->child[LEFT], val));
			};

			size_type count (const key_type& k) const
			{
				if (find(k).node == this->_sentinel)
					return (0);
				return (1);
			};

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
	bool operator<( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator<=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator>( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

	template< class Key, class T, class Compare, class Alloc >
	bool operator>=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs );

}
