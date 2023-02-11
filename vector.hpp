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

// Costruttore default (forse da togliere)
		explicit vector():
		_alloc(allocator_type()),
		_size(0),
		_capacity(0),
		_last(NULL),
		_begin(NULL),
		_end(NULL)
		{};

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

		template< class InputIt >
		vector( InputIt first, InputIt last, const allocator_type& alloc = allocator_type() ):
		_alloc(alloc),
		_size(0),
		_capacity(0),
		_last(NULL),
		_begin(NULL),
		_end(NULL)
		{
			
		};

		// vector( const vector& other );

		private:

		allocator_type	_alloc; //istanza dell'allocatore utilizzato per gestire la memoria utilizzata dal vettore
		size_type		_size; //dimensione attuale / numero di elementi memorizzati nel vettore
		size_type		_capacity; //dimensione massima che il vettore può raggiungere prima che sia necessario allocare più memoria
		pointer			_begin; //puntatore all'inizio del vettore
		pointer			_end; //puntatore alla fine del vettore
		pointer			_last; /*puntatore all'ultimo elemento allocato del vettore. Utile per sapere se è necessario allocare più memoria
							     quando si inseriscono nuovi elementi nel vettore. In questo modo, puoi verificare se c'è spazio sufficiente
							     per inserire nuovi elementi prima di allocare più memoria*/

	};
}
