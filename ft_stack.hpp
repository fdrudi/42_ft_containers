/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:35:25 by ddelladi          #+#    #+#             */
/*   Updated: 2022/11/29 17:37:45 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
		public:
			typedef				Container					container_type;
			typedef typename	Container::value_type		value_type;
			typedef typename	Container::size_type		size_type;
			typedef typename	Container::reference		reference;
			typedef typename	Container::const_reference	const_reference;

			explicit stack(const Container& cont = Container()) { c = cont; };
			stack(const stack& other) { c = other.c; };
			stack&	operator=(const stack& other)
			{
				if (this == &other)
					return (*this);
				this->c = other.c;
				return (*this);
			};
			virtual ~stack() {};


			// Element access
			
			reference		top() { return (c.back()); };
			const_reference	top() const { return (c.back()); };


			// Capacity

			bool		empty() const { return (c.empty()); };
			size_type	size() const { return (c.size()); };


			// Modifiers

			void	push(const value_type& value) { c.push_back(value); };
			void	pop() { c.pop_back(); };

			// Overloads

			friend bool	operator==(stack const & lhs, stack const & rhs) { return lhs.c == rhs.c; };
			friend bool	operator!=(stack const & lhs, stack const & rhs) { return lhs.c != rhs.c; };
			friend bool	operator<(stack const & lhs, stack const & rhs) { return lhs.c < rhs.c; };
			friend bool	operator<=(stack const & lhs, stack const & rhs) { return lhs.c <= rhs.c; };
			friend bool	operator>(stack const & lhs, stack const & rhs) { return lhs.c > rhs.c; };
			friend bool	operator>=(stack const & lhs, stack const & rhs) { return lhs.c >= rhs.c; };

		protected:
			Container	c;
	};
}
