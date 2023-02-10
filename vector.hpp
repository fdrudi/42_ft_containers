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

		vector()
		{};

		// explicit vector( const Allocator& alloc );

		// explicit vector( size_type count,
        //          const T& value = T(),
        //          const Allocator& alloc = Allocator());

		// template< class InputIt >
		// vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() );

		// vector( const vector& other );

		private:

		allocator_type	_alloc;
		size_type		_size;
		pointer			_max;
		pointer			_begin;
		pointer			_end;


	};
}
