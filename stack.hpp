#pragma once

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
		public:
			typedef 			Container					container_type;
			typedef typename	Container::value_type		value_type;
			typedef typename 	Container::size_type		size_type;
			typedef typename	Container::reference		reference;
			typedef typename	Container::const_reference	const_reference;


			// * COSTRUTTORI * //

			explicit stack (const Container& ctnr = Container())
			{
				_c = ctnr;
			}

			stack (const stack& other)
			{
				_c = other._c;
			}

			stack& operator=(const stack& other)
			{
				if (this == &other)
						return (*this);
				this->_c = other._c;
				return (*this);
			}

			~stack() {};


			// * MEMBER FUNCTION * //

			bool empty() const
			{
				return (_c.empty());
			};

			size_type size() const
			{
				return (_c.size());
			};

			reference& top()
			{
    			if (empty())
				{
        			throw std::out_of_range("stack::top(): empty stack");
    			}
    			return _c.back();
			};

			const_reference& top() const
			{
    			if (empty())
				{
        			throw std::out_of_range("stack::top(): empty stack");
    			}
    			return _c.back();
			}

			void push (const value_type& value)
			{
				_c.push_back(value);
			};

        	void pop()
			{
				_c.pop_back();
			};

			template <class V, class Cont>
			friend bool operator== (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);
			template <class V, class Cont>
			friend bool operator!= (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);
			template <class V, class Cont>
			friend bool operator<  (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);
			template <class V, class Cont>
			friend bool operator<= (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);
			template <class V, class Cont>
			friend bool operator>  (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);
			template <class V, class Cont>
			friend bool operator>= (const stack<V,Cont>& lhs, const stack<V,Cont>& rhs);

		protected:
			Container	_c;

	};

	template <class V, class Cont>
	bool operator==(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
		return (lhs._c == rhs._c);
	}

	template <class V, class Cont>
	bool operator!=(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
		return (lhs._c != rhs._c);
	}

	template <class V, class Cont>
	bool operator>=(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
		return (lhs._c >= rhs._c);
	}

	template <class V, class Cont>
	bool operator<(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
		return (lhs._c < rhs._c);
	}

	template <class V, class Cont>
	bool operator<=(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
        return (lhs._c <= rhs._c);
	}

	template <class V, class Cont>
	bool operator>(const ft::stack<V,Cont>& lhs, const ft::stack<V,Cont>& rhs)
	{
		return (lhs._c > rhs._c);
	}
}
