#pragma once

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
		public:

		typedef T				value_type;
		typedef Container		container_type;
		typedef std::size_t		size_type;


		// * COSTRUTTORI * //

		explicit stack (const container_type& ctnr = container_type())
		{
			c = ctnr;
		}

		virtual ~stack()
		{};


		// * MEMBER FUNCTION * //

		bool empty() const
		{
			return (_c.empty());
		};

		size_type size() const
		{
			return (_c.size());
		};

		value_type& top()
		{
			return (_c.back());
		};

		const value_type& top() const
		{
			return (_c.back());
		};

		void push (const value_type& val)
		{
			_c.push_back(val);
		};

        void pop()
		{
			_c.pop_back();
		};


		protected:

		container_type	_c;

	};

	template <class T, class Container>
	bool operator==(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
		return (lhs._c == rhs._c);
	}

	template <class T, class Container>
	bool operator!=(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
		return (lhs._c != rhs._c);
	}

	template <class T, class Container>
	bool operator>=(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
		return (lhs._c >= rhs._c)
	}

	template <class T, class Container>
	bool operator<(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
		return (lhs._c < rhs._c);
	}

	template <class T, class Container>
	bool operator<=(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
        return (lhs._c <= rhs._c);
	}

	template <class T, class Container>
	bool operator>(const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs)
	{
		return (lhs._c > rhs._c);
	}
}

