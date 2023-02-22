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

	/*Define a struct to represent a node in the RBTree. It stores a value, a color,
	  pointers to its parent and its left and right children. */
	template <typename T>
	struct Node
	{
		node_color	color;
		Node		*parent;
		Node		*child[2];
		T 			data;

		template <class U, class V> //Constructor for creating a node from a value
		Node(ft::pair<U, V> const & val) : data(val) {};
	};

	/* Define a struct to represent a node in the RBTree. It stores a key-value pair,
	   pointers to its parent and its left and right children, and its color. */
	template <class Pair>
	struct NodeRB2
	{
		NodeRB2					*parent;
		NodeRB2 				*left;
		NodeRB2 				*right;
		Pair					data;
		int						color;
	};

	/* Define a class to represent a Red-Black Tree (RBTree) with nodes of type NodeType,
	   keys of type Key, and values of type Value. The RBTree is implemented using a binary
	   search tree, and satisfies the properties of a red-black tree (e.g., every node is
	   either red or black, the root is black, no two adjacent nodes can be red, etc.).
	   The class also provides various methods for manipulating the tree, including
	   insert, remove, find, and traversal methods. */
	template <	class Key,
				class NodeType,
				class Iterator,
				class ConstIterator,
				class Compare = std::less<Key>,
				class Alloc = std::allocator<Key> >
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

		/* Questo è il costruttore di default della classe RBTree.
		   Viene inizializzato l'albero con la radice NULL, la dimensione 0 e un allocator di tipo allocator_type().
		   Viene inoltre allocato il sentinel, un elemento sentinella, che sarà utilizzato come elemento terminale dell'albero e colorato come SENTINEL.
		   Il sentinel viene poi impostato come radice dell'albero, e viene assegnato come padre della radice. */
		RBTree() :	_root(NULL),
						_size(0),
						_alloc(allocator_type())
		{
			_sentinel = _alloc.allocate(1);
			_sentinel->color = SENTINEL;
			_root = _sentinel;
			_sentinel->parent = _root;
		};

		/* Il costruttore di copia RBTree(RBTree const &src) crea una nuova istanza di RBTree come copia di src.
		   Inizializza il membro _alloc con un'istanza di allocator_type(), alloca un nodo sentinella _sentinel attraverso la funzione allocate() dell'allocator _alloc,
		   inizializza i campi di _sentinel con SENTINEL per il colore e con _root per il puntatore del genitore, quindi assegna _sentinel a _root e inizializza _size a 0.
		   Successivamente, itera sugli elementi di src usando l'iteratore iterator e inserisce ogni valore nell'albero attraverso la funzione insert().
		   Alla fine, viene creata una copia dell'albero src. */
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

		/* Questa è la definizione dell'operatore di assegnazione, ovvero l'operatore che permette di copiare una RBTree in un'altra.
		   L'operatore è ridefinito perché, essendo una struttura di dati complessa, la copia bit a bit dei dati non è sufficiente per effettuare una copia corretta.
		   Il codice controlla innanzitutto che gli oggetti non siano gli stessi (in tal caso ritorna il puntatore all'oggetto corrente).
		   Dopodiché, copia l'allocator dall'oggetto passato come parametro, alloca l'oggetto _sentinel, lo imposta come sentinella (SENTELIN),
		   ne assegna il genitore a _root e imposta il valore di _size a 0. Infine, chiama la funzione insert per ogni elemento nella RBTree passata come parametro, copiandoli tutti nella RBTree corrente.
		   Il valore di ritorno è l'oggetto RBTree corrente, perché in C++ l'operatore di assegnazione deve restituire un riferimento all'oggetto che viene assegnato. */
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

		/*Libera la memoria allocata per il nodo sentinella.*/
		~RBTree() { _alloc.deallocate(_sentinel, 1); };

		/* Restituisce l'allocator utilizzato per allocare la memoria per il RBTree. */
		allocator_type	get_allocator() const { return (this->_alloc); }

		/* Restituisce true se il RBTree è vuoto, false altrimenti. */
		bool empty() const { return ((!this->_size) ? true : false); }

		/* Restituisce il numero di elementi presenti nel RBTree. */
		size_type size() const { return this->_size; }

		/* Restituisce il massimo numero di elementi che il RBTree può contenere in base alla politica di allocazione della memoria. */
		size_type max_size() const { return _alloc2.max_size(); }


		/* Restituiscono degli iteratori che permettono di iterare tra gli elementi del RBTree. */
		iterator				begin() { return (iterator(min(), _sentinel)); }
		const_iterator			begin() const { return (const_iterator(min(), _sentinel)); }
		iterator				end() { return iterator(_sentinel, _sentinel); }
		const_iterator			end() const { return const_iterator(_sentinel, _sentinel); }
		reverse_iterator		rbegin() { return (reverse_iterator(end())); }
		const_reverse_iterator	rbegin() const { return (const_iterator(end())); }
		reverse_iterator		rend() { return (reverse_iterator(begin())); }
		const_reverse_iterator	rend() const { return (const_reverse_iterator(begin())); }

		/* Questo metodo accetta un puntatore a nodo e un valore di chiave e cerca ricorsivamente il nodo nell'albero con quella chiave.
		   Restituisce un iteratore al nodo se trovato, altrimenti restituisce un iteratore alla fine. */
		virtual iterator					findPointer(pointer& start, Key const & val) const = 0;

		/* Questo metodo accetta un valore di chiave e rimuove dal sottoalbero radicato al nodo con la chiave specificata tutti i nodi con quella chiave.
		   Restituisce il numero di nodi rimossi. */
		virtual iterator					erase_deep(Key const & val) = 0;

		/* questo metodo accetta un valore di chiave e inserisce un nuovo nodo nell'albero con quella chiave.
		   Restituisce un oggetto pair composto da un iteratore al nodo appena inserito
		   e un flag booleano che indica se l'inserimento è avvenuto con successo o se la chiave era già presente nell'albero. */
		virtual ft::pair<iterator, bool>	insert(Key const &val) = 0;

		/* Questo metodo accetta tre puntatori a nodi e un flag.
		   Inserisce il nodo node nell'albero come figlio del nodo parent, avendo come figlio destro o sinistro a seconda del valore di flag.
		   Il parametro start è il puntatore alla radice dell'albero. Restituisce un oggetto pair composto da un iteratore al nodo appena inserito e un flag booleano che indica se l'inserimento è avvenuto con successo.*/
		virtual ft::pair<iterator, bool>	insertNode(pointer &start, pointer &node, pointer& parent, int flag) = 0;

		virtual void						clear() = 0;

		/* La funzione controlla se il nodo passato come argomento è il nodo più piccolo dell'albero.
		   In questo caso, restituisce un puntatore al sentinella, che rappresenta il minimo valore nell'albero.
		   Se il nodo non è il minimo, cerca il massimo valore nel sottoalbero sinistro del nodo.
		   Se esiste, restituisce il massimo valore. Se non esiste, la funzione risale l'albero risalendo i genitori
		   fino a trovare il primo genitore che ha il figlio destro uguale al nodo passato come argomento e restituisce questo genitore. */
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

		/* La funzione controlla se il nodo passato come parametro è il massimo della pianta: in questo caso restituisce il sentinella.
		   Altrimenti, se il figlio destro del nodo non è il sentinella, restituisce il nodo minimo del sottoalbero radicato in quel figlio destro.
		   In caso contrario, cerca il primo genitore a sinistra e restituisce tale nodo, oppure il sentinella se non trova un genitore a sinistra.
		   Il comportamento di getPredecessor è simile, ma cerca il predecessore invece del successore del nodo passato come parametro. */
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

		/* In questa implementazione, viene innanzitutto inizializzato un puntatore node all'indirizzo della radice _root.
		   Se l'albero è vuoto o la radice è il sentinella, viene restituito il puntatore al sentinella.
		   In caso contrario, il ciclo while viene eseguito finché il puntatore node non punta a un nodo che non ha un figlio sinistro oppure il figlio sinistro è il sentinella.
		   In questo caso, il puntatore viene aggiornato al puntatore al figlio sinistro del nodo corrente.
		   Quando il ciclo termina, il puntatore node punta al nodo con la chiave minima e viene restituito il suo valore. */
		pointer	min() const
		{
			const pointer*	node = &_root;

			if (!(*node) || (*node) == _sentinel)
				return (_sentinel);

			while ((*node)->child[LEFT] && (*node)->child[LEFT] != _sentinel)
				node = &(*node)->child[LEFT];
			return (*node);
		}

		/* La funzione 'min' prende un puntatore a un nodo dell'albero e restituisce un puntatore al nodo con la chiave minima dell'albero il cui nodo radice è il nodo passato come parametro.
		   In pratica, la funzione si sposta lungo il sottoalbero sinistro del nodo passato come parametro finché non raggiunge un nodo che non ha figli sinistri (o il sentinella), 
		   e restituisce tale nodo. 
		   Se il nodo passato come parametro è il minimo dell'albero, allora viene restituito il suo stesso puntatore.
		   La funzione usa un puntatore const a un puntatore non const in modo da poter modificare il puntatore indiretto senza modificare il valore del puntatore originale. */
		pointer	min(pointer const & node) const
		{
			const pointer*	tmp = &node;

			if (!(*tmp) || (*tmp) == _sentinel)
				return (_sentinel);

			while ((*tmp)->child[LEFT] && (*tmp)->child[LEFT] != _sentinel)
				tmp = &(*tmp)->child[LEFT];
			return (*tmp);
		}

		/* La funzione 'max' restituisce un puntatore all'elemento con la chiave massima nell'albero.
		   Inizialmente, viene inizializzato un puntatore node con la radice dell'albero _root. 
		   Se la radice è null o _sentinel, l'albero è vuoto e viene restituito il puntatore _sentinel.
		   Altrimenti, si itera sull'albero a partire dalla radice, seguendo sempre il figlio destro finché esiste un figlio destro e non è il _sentinel. 
		   Quando non è più possibile seguire il figlio destro, l'iterazione termina e viene restituito il puntatore al nodo trovato come massimo. */
		pointer	max() const
		{
			pointer	node = _root;

			if (!node || node == _sentinel)
				return (_sentinel);

			while (node->child[RIGHT] && node->child[RIGHT] != _sentinel)
				node = node->child[RIGHT];
			return (node);
		}

		/* La funzione riceve un parametro node che rappresenta la radice del sottoalbero in cui cercare il valore massimo.
		   Inizia esaminando il nodo di partenza, e finché esiste un figlio destro, la funzione continua a scendere nell'albero, selezionando ogni volta il figlio destro. 
		   Quando non esiste più un figlio destro, allora il nodo corrente è il massimo del sottoalbero radicato in node, e viene restituito. 
		   Se node è un puntatore nullo o punta al nodo sentinella, la funzione restituisce il puntatore al nodo sentinella. */
		pointer	max(pointer const & node) const
		{
			pointer	tmp = node;

			if (!node || node == _sentinel)
				return (_sentinel);

			while (tmp->child[RIGHT] != _sentinel)
				tmp = tmp->child[RIGHT];
			return (tmp);
		}

		/* Questo metodo scambia i contenuti dell'oggetto corrente con quelli di un altro oggetto RBTree passato per riferimento come parametro.
		   Il metodo inizia con un controllo per evitare di scambiare l'oggetto corrente con se stesso, quindi salva temporaneamente i membri dell'oggetto corrente 
		   (radice, sentinella, dimensione, tipo di allocazione e comparatore) in variabili temporanee. 
		   Successivamente, i membri dell'oggetto corrente vengono sovrascritti con i membri dell'oggetto rhs. 
		   Infine, i membri dell'oggetto rhs vengono sovrascritti con i valori temporanei salvati in precedenza dall'oggetto corrente.
		   In questo modo, alla fine dell'esecuzione, l'oggetto corrente e l'oggetto rhs hanno scambiato i loro contenuti. */
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

		/* Questa è una funzione privata della classe RBTree che prende un nodo e restituisce il suo unico figlio, se tale figlio non ha figli a sua volta (ovvero è una foglia).
		   La funzione controlla se il nodo ha un figlio sinistro ma non destro o viceversa, in tal caso restituisce il figlio che soddisfa questa condizione. 
		   Se il nodo ha entrambi i figli o nessun figlio, la funzione restituisce NULL.
		   Successivamente, viene controllato se il figlio restituito ha a sua volta figli. Se il figlio ha figli, la funzione restituisce NULL. Altrimenti, la funzione restituisce il figlio.
		   Questa funzione viene utilizzata internamente dalla classe RBTree durante la cancellazione di un nodo con zero o un solo figlio. 
		   Se il nodo ha un solo figlio, il figlio viene utilizzato per sostituire il nodo cancellato. 
		   La funzione getOnlyChildWithoutChildren viene utilizzata per controllare se il figlio del nodo cancellato è effettivamente una foglia. 
		   In caso contrario, la cancellazione non può essere eseguita e viene restituito un errore. */
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
