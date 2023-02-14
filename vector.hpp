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
		_last(NULL),
		_begin(NULL),
		_end(NULL)
		{};

// Costruttore con 'count' copie dell'elemento 'val'
		explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type()):
		_alloc(alloc),
		_size(0),
		_capacity(0),
		_last(NULL),
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
				_last = _end;
				_last--;
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
			_last = _end;
			_last--;
		};

// Copy Constructor
		vector( const vector& other ):
		_alloc(other._alloc),
		_size(other._size),
		_capacity(other._capacity),
		_begin(other._alloc.allocate(other._capacity)),
		{

			if (!other.empty())
				assign(other.begin(), other.end()); // DA IMPLEMENTARE
			_end = _begin + _size;
			_last = _end - 1;
		};

// Copy Assign operator
		vector&	operator=(const vector &other)
		{
			if (other == *this)
				return (*this);
			this->clear(); // DA IMPLEMENTARE (destroy chiama il distruttore dell'oggetto all'interno del vettore)
			if (this->capacity())
			{
				_alloc.deallocate(_begin, _capacity); // (deallocate libera la memoria allocata)
				_begin = _alloc.allocate(other.capacity());
				_capacity = other.capacity();
				_end = _begin;
			}
			this->insert(this->end(), other.begin(), other.end()); // DA IMPLEMENTARE
			_last = _end - 1;
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

// Iteratori

		iterator				begin()			{ return (iterator(_begin)); };
		const_iterator			begin() const	{ return (const_iterator(_begin)); };
		iterator				end()			{ return (iterator(_begin + _size)) };
		const_iterator			end() const		{ return (const_iterator(_begin + _size)) };
		reverse_iterator		rbegin()		{ return (reverse_iterator(end())) };			// essendo reverse,
		const_reverse_iterator	rbegin() const	{ return (const_reverse_iterator(end())) };		// ragiona al contrario.
		reverse_iterator		rend()			{ return (reverse_iterator(begin())) };			// A mio parere
		const_reverse_iterator	rend() const	{ return (const_reverse_iterator(begin())) };	// totalmente inutile.
																								//		Cit. sisittu
// Capacity

		size_type		size() const { return (_size); };
		size_type		max_size() const { return (allocator_type().max_size()); }; // ritorna la massima capacità ipotetica che si può dare come argomento a reserve().
		void 			resize (size_type n, value_type val = value_type())
		{
			size_type	new_cap
		};

		size_type		capacity() const { return (_capacity); };
		bool			empty() const { return (_size == 0 ? true : false); };

		void reserve(size_type n) // riserva spazio allocato per n elementi
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
			_last = _begin + n;
			if (tmp_begin && tmp_begin != _begin) // elimina lo spazio predefinito e non più utilizzato
				_alloc.deallocate(tmp_begin, prev_capacity);
		};
		

// Element access

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

		/*
			Ciao ragazzuoli,

			Il vettore è praticamente pronto. Mancano però i modifiers che sono palesemente una rogna,
			e visto che sono davvero di buon cuore nono vi ho voluto togliere il divertimento per questi due giorni.
			D'altronde adesso sono le 00:57, domani è San Valentino e se non mi sveglio ad un orario decente
			verrò linciato dalla signora.
			Nel dubbio, usate questo messaggio come mio testamento: sappiate che vi ho voluto bene,
			che i miei beni vanno devoluti per il pagamento dei miei debiti e ciò che avanza servirà per pagare
			le pratiche burocratiche per seppellirmi in terra straniera extracomunitaria.

			XOXO, il vostro buon vecchio sisittu.
		*/

		private:

		allocator_type	_alloc; //istanza dell'allocatore utilizzato per gestire la memoria utilizzata dal vettore
		size_type		_size; //dimensione attuale / numero di elementi memorizzati nel vettore
		size_type		_capacity; //dimensione massima che il vettore può raggiungere prima che sia necessario allocare più memoria
		pointer			_begin; //puntatore all'inizio del vettore
		pointer			_end; //puntatore alla fine del vettore
		pointer			_last; /* puntatore all'ultimo elemento allocato del vettore. Utile per sapere se è necessario allocare più memoria
							     quando si inseriscono nuovi elementi nel vettore. In questo modo, puoi verificare se c'è spazio sufficiente
							     per inserire nuovi elementi prima di allocare più memoria */

	};
}
