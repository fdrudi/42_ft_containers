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
			typedef	ft::pair<const key_type, mapped_type>								value_type;
			typedef typename	Allocator::template rebind<Node<value_type> >::other	allocator_type;
			typedef RBIterator<value_type, Compare, Node<value_type> >					iterator;
			typedef RBIteratorConst<value_type, Compare, Node<value_type> >				const_iterator;
			typedef typename allocator_type::pointer									pointer;



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

			/*	Inserisce un valore all'interno dell'albero. Dal valore crea un nodo e controlla
				se la mappa è vuota e in caso imposta il nodo come nuova root.
				Altrimenti controlla la prima direzione da prendere dalla root in modo da avere
				un parent da utilizzare nella funzione insertNode. Altrimenti elimina il nodo
				e ritorna false. */
			std::pair<iterator, bool> insert( const value_type& value )
			{
				ft::pair<iterator, bool>	dst;
				pointer node = new Node<value_type>(val);

				node->color = RED;
				node->parent = this->_sentinel;
				node->child[LEFT] = this->_sentinel;
				node->child[RIGHT] = this->_sentinel;

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
					if (this->_c(val.first, this->_root->data.first) && this->_root->data.first != val.first)
						return (insertNode(this->_root->child[LEFT], node, this->_root, 1));
					else if (this->_c(this->_root->data.first, val.first) && this->_root->data.first != val.first)
						return (insertNode(this->_root->child[RIGHT], node, this->_root, 1));
					else
					{
						dst.first = find(node->data.first);
						delete node;
						dst.second = false;
						return (dst);
					}
				}
			};

			/* Inserisce un range di nodi */
			template< class InputIt >
			void insert( InputIt first, InputIt last )
			{
				while (first != last)
					this->insert(*first++);
			};

	//!	iterator insert (iterator position, const value_type& val)
		/*	{
				ft::pair<iterator, bool> dst;

				(void)position;
				dst this->insert(val);
				return (dst.first);
			};	*/

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

			void	erase(iterator pos)
			{
				this->erase_deep(*pos);
			}

			void	erase(iterator first, iterator last)
			{
				while (first != last)
					this->erase_deep(*first++);
			}

			size_type	erase(const key_type& key)
			{
				ft::pair<key_type, mapped_type>	tmp(key, mapped_type());

				if (erase_deep(tmp) != iterator(NULL, this->_sentinel))
					return (1);
				return (0);
			}

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
				// La funzione cerca il nodo dell'albero che corrisponde alla chiave specificata e lo memorizza nella variabile "node".
				pointer		node = this->find(val.first).node;
				pointer		temp;
				pointer		successor;
				iterator 	ret;

				/* Viene controllato se il nodo esiste nell'albero e non è il nodo sentinella dell'albero,
				   altrimenti viene restituita un'istanza dell'iteratore con valore nullo e puntatore al nodo sentinella. */
				if (node && node != this->_sentinel)
					ret = this->find(this->getSuccessor(node)->data.first);
				else
					return (iterator(NULL, this->_sentinel));

				/* Se il nodo non ha figli o ha entrambi i figli come nodi sentinella,
				   viene eseguita la cancellazione del nodo. Viene controllato se il nodo cancellato è nero e se non è la radice dell'albero.
				   In questo caso, viene richiamata la funzione "balanceDelete" per bilanciare l'albero.
				   Successivamente, viene eseguita la disconnessione del nodo dall'albero tramite la funzione "unlink"
				   se il padre del nodo è diverso dal nodo sentinella. Altrimenti, viene impostata la radice dell'albero come il nodo sentinella. */
				if ((!node->child[LEFT] && !node->child[RIGHT]) || (node->child[LEFT] == this->_sentinel && node->child[RIGHT] == this->_sentinel))
				{
					temp = node->parent;
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
				/* Se il nodo ha un solo figlio, viene eseguita la cancellazione del nodo.
				   Viene controllato se il nodo cancellato è nero e se non è la radice dell'albero.
				   In questo caso, viene richiamata la funzione "balanceDelete" per bilanciare l'albero.
				   Il figlio del nodo viene collegato al padre del nodo cancellato tramite la funzione "link" se il nodo cancellato non è la radice dell'albero.
				   Altrimenti, viene impostata la radice dell'albero come il figlio del nodo cancellato. */
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
				/* Se il nodo ha entrambi i figli, viene eseguita la cancellazione del nodo.
				   Viene selezionato il successore del nodo cancellato.
				   Se il successore ha un figlio destro diverso dal nodo sentinella,
				   viene impostato a questo il padre del successore tramite la funzione "unlink".
				   Successivamente, il successore viene collegato al padre del nodo cancellato tramite la funzione "link".
				   Infine, i figli del nodo cancellato vengono collegati al successore.
				   Se il nodo cancellato è la radice dell'albero,
				   viene eseguita una procedura speciale per collegare correttamente il figlio del nodo cancellato con il nuovo nodo radice.*/
				else
				{
					if (this->_c(node->data.first, this->_root->data.first))
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
				this->_alloc.deallocate(node, 1); //Viene deallocato il nodo cancellato e decrementato il valore della variabile _size che tiene traccia della grandezza dell'albero.
				node = NULL;
				this->_size--;
				return (ret); // Viene restituito l'iteratore che punta al successore del nodo cancellato.
			};

			void clear()
			{
				while(this->_size)
					this->erase(this->min());
			};

			class value_compare : public std::binary_function<value_type,value_type,bool>
			{
				friend class map<Key, T>;

				protected:
					Compare comp;
					value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
				public:
					bool operator() (const value_type& x, const value_type& y) const
					{
						return comp(x.first, y.first);
					}
			};

			/* ritorna un oggetto funzione [vedi class value_compare].
			   Costruisce una funzione di comparazione basata sul template Compare (solitamente assegnato a std::less) */
			value_compare value_comp() const
			{
				return (value_compare(this->key_comp()));
			};

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

			/* controlla l'esistenza della key all'interno della map. */
			size_type count (const key_type& k) const
			{
				if (find(k).node == this->_sentinel)
					return (0);
				return (1);
			};

			/* ritorna il primo elemento maggiore (basandosi sugli iteratori) della map. */
			iterator lower_bound (const key_type& k)
			{
				iterator	tmp = this->begin();

				while (tmp != this->end() && this->_c(tmp.node->data.first, k))
					tmp++;
				return (tmp);
			};

			const_iterator lower_bound (const key_type& k) const
			{
				const_iterator	tmp = this->begin();

				while (tmp != this->end() && this->_c(tmp.node->data.first, k))
					tmp++;
				return (tmp);
			};

			/* restituisce un iteratore all'elemento con la key strettamente maggiore della key data. */
			iterator upper_bound (const key_type& k)
			{
				iterator	tmp = this->begin();

				while (tmp != this->end() && !this->_c(k, tmp.node->data.first))
					tmp++;
				return (tmp);
			};

			const_iterator upper_bound (const key_type& k) const;
			{
				const_iterator	tmp = this->begin();

				while (tmp != this->end() && !this->_c(k, tmp.node->data.first))
					tmp++;
				return (tmp);
			};

			/*	ritorna un pair con il primo elemento avente la key data e il primo elemento maggiore.
				Se il first e il second sono uguali, allora non esistono elementi con la chiave data. */
			ft::pair<iterator,iterator> equal_range (const key_type& k)
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			ft::pair<const_iterator,const_iterator> equal_range (const key_type& k) const;
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator==( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return ((lhs.size() == rhs.size()) && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator!=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return (!(lhs == rhs));
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator<( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator<=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return (!(lhs > rhs));
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator>( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return (!(lhs == rhs || lhs < rhs));
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator>=( const std::map<Key, T, Compare, Alloc>& lhs, const std::map<Key, T, Compare, Alloc>& rhs )
	{
		return (!(lhs < rhs));
	};

}
