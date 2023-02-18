#include <memory>
#include "iterator.hpp"

namespace ft
{
	template< class T, class Allocator = std::allocator<T> >
	class vector
	{
		public:

		typedef	T													value_type;
		typedef	Allocator											allocator_type;
		typedef	std::size_t											size_type;
		typedef	std::ptrdiff_t										difference_type;
		typedef	value_type&											reference;
		typedef	const value_type&									const_reference;
		typedef	typename allocator_type::pointer					pointer;
		typedef	typename allocator_type::const_pointer				const_pointer;
		typedef	ft::random_access_iterator<value_type>				iterator;
		typedef	ft::random_access_iterator<const value_type>		const_iterator;
		typedef	ft::reverse_iterator<iterator>						reverse_iterator;
		typedef	ft::reverse_iterator<const_iterator>				const_reverse_iterator;

		// * COSTRUTTORI * //

		// Costruttore con tipo di Allocator
		explicit vector(const allocator_type& alloc = allocator_type()):
		_alloc(alloc),
		_size(0),
		_capacity(0),
		_begin(NULL),
		_end(NULL)
		{};

		// Costruttore con 'count' copie dell'elemento 'val'
		explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type()):
		_alloc(alloc),
		_size(0),
		_capacity(0),
		_begin(NULL),
		_end(NULL)
		{
			if (n >= 0 && n < this->max_size())
			{
				_size = n;
				_capacity = n;
				_begin = _alloc.allocate(_capacity);
				_end = _begin;
				while (n--)
					_alloc.construct(_end++, val);
			}
			else
				throw std::length_error("vector");
		};

		// Costruttore con range da 'first' a 'last'
		template< class InputIt >																		// Controllo eseguito al run-time se il type è integral
		vector( InputIt first, InputIt last, const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type * = 0 ):
		_alloc(alloc),
		{
			bool is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value;
			if (!is_valid)
				throw ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::type>();
			difference_type n = ft::distance(first, last);
			_size = n;
			_capacity = n;
			_begin = _alloc.allocate(_capacity);
			_end = _begin;
			while (n--)
			{
				_alloc.construct(_end, *first);
				_end++;
				first++;
			}
		};

		// Copy Constructor
		vector( const vector& other ):
		_alloc(other._alloc),
		_size(other._size),
		_capacity(other._capacity),
		_begin(other._alloc.allocate(other._capacity)),
		{

			if (!other.empty())
				assign(other.begin(), other.end());
			_end = _begin + _size;
		};

		// Copy Assign operator
		vector&	operator=(const vector &other)
		{
			if (other == *this)
				return (*this);
			this->clear();
			if (this->capacity())
			{
				_alloc.deallocate(_begin, _capacity);
				_begin = _alloc.allocate(other.capacity());
				_capacity = other.capacity();
				_end = _begin;
			}
			this->insert(this->end(), other.begin(), other.end());
			return (*this);
		};

		// Distruttore
		~vector()
		{
			this->clear();
			if (this->_begin != NULL || _capacity != 0)
				_alloc.deallocate(_begin, _capacity);
		}

		// * MEMBER FUNCTION *//

		// ITERATORI

		iterator				begin()			{ return (iterator(_begin)); }; // Getter Iteratore che punta all'inizio
		const_iterator			begin() const	{ return (const_iterator(_begin)); }; // Getter Iteratore Const che punta all'inizio
		iterator				end()			{ return (iterator(_begin + _size)) }; // Getter Iteratore che punta alla fine
		const_iterator			end() const		{ return (const_iterator(_begin + _size)) }; // Getter Iteratore Const che punta alla fine
		reverse_iterator		rbegin()		{ return (reverse_iterator(end())) }; // Getter Iteratore Reverse che punta all'inizio
		const_reverse_iterator	rbegin() const	{ return (const_reverse_iterator(end())) }; // Getter Iteratore Reverse Const che punta all'inizio
		reverse_iterator		rend()			{ return (reverse_iterator(begin())) }; // Getter Iteratore Reverse che punta alla fine
		const_reverse_iterator	rend() const	{ return (const_reverse_iterator(begin())) }; // Getter Iteratore Reverse Const che punta alla fine

		// CAPACITY

		size_type		size() const { return (_size); };
		size_type		max_size() const { return (allocator_type().max_size()); }; // ritorna la massima capacità ipotetica che si può dare come argomento a reserve().

		/*
			ridefinisce la size del container prendendo da argomento il nuovo numero (n) di elementi;
			se n è maggiore del precedente, inserirà in ultima posizione dei nuovi elementi fino ad arrivare a n
			se n è minore, rimuoverà dall'ultimo fino al necessario per arrivare ad n
		*/
		void 			resize(size_type n, value_type val = value_type())
		{
			size_type	new_cap = this->capacity();

			if (n > this->max_size())
				throw std::length_error("vector::resize");
			if (n > this->capacity())
			{
				while (n > new_cap)
					new_cap *= 2;
				this->reserve(new_cap);
				this->insert(this->end(), n - this->size(), val);
			}
			else if (n > this->size())
				this->insert(this->end(), n - this->size(), val);
			else if (n == this->size())
				return ;
			else
			{
				for (size_type i = _size - n; i > 0; i--)
				{
					_alloc.destroy(_end--);
					_size--;
				}
			}
		}

		// Getter Capacity
		size_type		capacity() const
		{
			return (_capacity);
		};

		// Controlla se la 'size' è vuota
		bool			empty() const
		{
			return (_size == 0 ? true : false);
		};

		// Riserva spazio allocato per n elementi
		void reserve(size_type n)
		{
			pointer	prev_begin;
			pointer prev_end;
			pointer	tmp_begin;
			size_type	prev_capacity;

			/* gestione errori */
			if (_capacity >= n)
				return ;
			if (n > max_size())
				throw std::length_error("ft::vector::reserve()");

			prev_begin = _begin;
			tmp_begin = prev_begin;
			prev_end = _end;
			prev_capacity = _capacity;

			/* allocazione ed eventuale cambio indirizzo di memoria*/
			_begin = _alloc.allocate(n, 0);
			_capacity = n;
			_end = _begin;

			while (prev_begin != prev_end) // parte a copiare gli elementi solo se esistevano già
			{
				_alloc.construct(_end++, *prev_begin); // inserisce nel punto _end il contenuto del punto prev_begin
				prev_begin++;
			}
			if (tmp_begin && tmp_begin != _begin) // elimina lo spazio predefinito e non più utilizzato
				_alloc.deallocate(tmp_begin, prev_capacity);
		};


		// ELEMENT ACCESS

		/*
			NB: per gli operator[], la documentazione parla di "undefined behaviour"
			nel caso di utilizzo improprio del parametro n.
		*/
		reference		operator[] (size_type n)		{ return (*(_begin + n))};
		const_reference	operator[] (size_type n) const	{ return (*(_begin + n))};

		reference		at(size_type n)
		{
			if (n < 0 || n > _size)
				throw	std::out_of_range("ft::vector::at()");
			return (*(_begin + n));
		};

		const_reference	at(size_type n) const
		{
			if (n < 0 || n > _size)
				throw	std::out_of_range("ft::vector::at()");
			return (*(_begin + n));
		};

		reference			front()			{ return (*_begin); };
		const_reference		front() const	{ return (*_begin); };
		reference			back()			{ return (*(_begin + _size)); };
		const_reference		back() const	{ return (*(_begin + _size)); };
		value_type*			data()			{ return (_begin); };	// ritorna il vettore
		const value_type*	data() const	{ return (_begin); };	// sotto forma di array puntatore

		// MODIFIERS
		/* assegna nuovi valori elemento per elemento in sequenza, nell'intervallo [first, last) */
		template <class InputIterator>
		void assign (InputIterator first, InputIterator last), 
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = 0)
		{
			bool	is_valid;
			is_valid = 
		}

		/* assegna il valore val per un numero (n) di elementi */
		void assign (size_type n, const value_type& val)
		{
			this->clear();
			if(n > this->capacity())
				this->reserve(n);
			this->insert(this->begin, n, val);
		};

		// inserisce un elemento in ultima posizione
		void push_back (const value_type& val)
		{
			if (_size == _capacity && _capacity + 1 > this->max_size())
				throw std::length_error("ft::vector::push_back");

			if (_size == _capacity)
			{
				if (_capacity == 0)
					this->reserve(1);
				else
					this->reserve(_capacity * 2);
			}
			_alloc.construct(_end++, val);
			_size++;
			return ;
		};

		/* inserisce un valore nel punto position */
		iterator insert (iterator position, const value_type& val) //Controllo dei valori con "enable_if"?
		{
			size_type	dist = position - this->begin();
			if (_size == _capacity)
			{
				if (_capacity == 0)
					this->reserve(1);
				else
					this->reserve(_capacity * 2);
				position = this->begin() + dist;
			}
			_end++;
			dist = _end - position + 1;
			size_type i = _size + 1;
			while (dist--) // spostamento dei dati precedentemente inseriti
			{
				_alloc.construct(_begin + i, _begin[i - 1]);
				i--;
			}
			_alloc.construct(position, val);
			_size++;
			return (position);
		};

		/* inserisce n valori uguali consecutivi a partire da position */
		void insert (iterator position, size_type n, const value_type& val) //Controllo dei valori con "enable_if"?
		{
			size_type	dist = position - this->begin();
			if (_size + n > _capacity)
			{
				if (_capacity == 0)
					this->reserve(n);
				else
				{
					while (size + n > _capacity)
						this->reserve(_capacity * 2);
				}
				position = this->begin() + dist;
			}
			_end += n;
			dist = _end - position + 1;
			size_type i = _size + n;
			while (dist--) // spostamento dei dati precedentemente inseriti
			{
				_alloc.construct(_begin + i, _begin[i - 1]);
				i--;
			}
			for (int j = 0; j < n; j++)
				_alloc.construct(position + j, val);
			_size += n;
			return (position);
		};

		/*
			dati due iteratori dello stesso tipo,
			inserisce i valori trovati dal punto first al punto last
			nel punto position appartenente al vettore.
		*/
		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last,
						typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = 0)
		{
			// controllo sul tipo di variabile valido o meno
			bool is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value;
			if (!is_valid)
				throw ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::type>();

			const size_type	n = ft::distance(first, last);
			size_type		dist = position - this->begin();
			size_type		finalSize = _size - n;

			if (finalSize > _capacity)
			{
				if (_capacity == 0)
					this->reserve(n);
				else
				{
					while (finalSize > _capacity)
						this->reserve(_capacity * 2);
				}
				position = this->begin() + dist;
				_end = _begin + _size;
			}
			dist = ft::distance(position, this->end());
			_end = _begin + finalSize;
			size_type i = finalSize;
			while (dist--) // spostamento dei dati precedentemente inseriti
			{
				_alloc.construct(_begin + i, _begin[i - 1]);
				i--;
			}
			for (int j = 0; j < n; j++)
				_alloc.construct(position + j, first++);
			_size = finalSize;
			return ;
		};

		/* elimina il valore nel punto position, spostando tutti gli altri elementi */
		iterator erase (iterator position)
		{
			size_type	i = position - this->begin();
			while (i + 1 < this->size())
			{
				_begin[i] = _begin[i + 1];
				i++;
			}
			_alloc.destroy(_end--);
			_size--;
			return (position);
		};

		/* elimina i valori dal punto first al punto last, spostando tutti gli altri elementi */
		iterator erase (iterator first, iterator last)
		{
			if(_end == last)
			{
				_size = first - _begin;
				_end = first;
				size_type	dist = ft::distance(first, last);
				size_type	i = last - _begin;
				while(dist--)
					_alloc.destroy(_begin + i--);
				return (this->end());
			}
			iterator	ret = first;
			_size -= ft::distance(first, last);
			while (last != this->end())
			{
					_alloc.construct(first.pointed(), *last++);
					first++;
			}

			// pulizia ultimi elementi non utilizzati
			_end = _begin + _size;
			while (first != last)
				_alloc.destroy(first++);
			return (ret);
		}

		// Scambia il contenuto e la lungezza del vettore con il vettore passato come parametro, utilizzando i puntatori
		void swap (vector& x)
		{
			pointer					tmpBegin = x._begin;
			pointer					tmpEnd = x._end;
			size_type				tmpCapacity = x._capacity;
			size_type				tmpSize = x._size;
			allocator_type			tmpAlloc = x._alloc;

			if (*this == x)
				return ;

			x._begin = this->_begin;
			x._end = this->_end;
			x._capacity = this->_capacity;
			x._size = this->_size;
			x._alloc = this->_alloc;
			this->_begin = tmpBegin;
			this->_end = tmpEnd;
			this->_capacity = tmpCapacity;
			this->_size = tmpSize;
			this->_alloc = tmpAlloc;
		}

		// Chiama il distruttore di ogni oggetto all' interno del vettore
		void clear()
		{
			while(_size)
			{
				_alloc.destroy(_end);
				_size--;
				_end--;
			}
		}

		private:

		allocator_type	_alloc; //istanza dell'allocatore utilizzato per gestire la memoria utilizzata dal vettore
		size_type		_size; //dimensione attuale / numero di elementi memorizzati nel vettore
		size_type		_capacity; //dimensione massima che il vettore può raggiungere prima che sia necessario allocare più memoria
		pointer			_begin; //puntatore all'inizio del vettore
		pointer			_end; //puntatore alla fine del vettore
	};
}
