/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:03:04 by ddelladi          #+#    #+#             */
/*   Updated: 2022/12/05 16:58:09 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <typeinfo>
#include <string>

namespace ft
{
	template <bool is_integral, typename T>
	struct is_integral_res
	{
		typedef T			type;
		static const bool	value = is_integral;
		operator type() const { return value; }
	};

	template <typename>
	struct is_integral_type : public is_integral_res<false, bool> {};

	template <>
	struct is_integral_type<bool> : public is_integral_res<true, bool> {};

	template <>
	struct is_integral_type<char> : public is_integral_res<true, char> {};

	template <>
	struct is_integral_type<wchar_t> : public is_integral_res<true, wchar_t> {};

	template <>
	struct is_integral_type<char16_t> : public is_integral_res<true, char16_t> {};

	template <>
	struct is_integral_type<signed char> : public is_integral_res<true, signed char> {};

	template <>
	struct is_integral_type<short int> : public is_integral_res<true, short int> {};

	template <>
	struct is_integral_type<int> : public is_integral_res<true, int> {};

	template <>
	struct is_integral_type<long int> : public is_integral_res<true, long int> {};

	template <>
	struct is_integral_type<long long int> : public is_integral_res<true, long long int> {};

	template <>
	struct is_integral_type<unsigned char> : public is_integral_res<true, unsigned char> {};

	template <>
	struct is_integral_type<unsigned short int> : public is_integral_res<true, unsigned short int> {};

	template <>
	struct is_integral_type<unsigned int> : public is_integral_res<true, unsigned int> {};

	template <>
	struct is_integral_type<unsigned long int> : public is_integral_res<true, unsigned long int> {};

	template <>
	struct is_integral_type<unsigned long long int> : public is_integral_res<true, unsigned long long int> {};

	template <typename T>
	struct is_integral : public is_integral_type<T> {};

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag	{};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag : public random_access_iterator_tag {};
	
	template <bool is_valid, typename T>
	struct valid_iterator_tag_res
	{
		typedef T			type;
		const static bool	value = is_valid;
	};

	template <typename T>
	struct is_input_iterator_tagged : public valid_iterator_tag_res<false, T> {};

	template <>
	struct is_input_iterator_tagged<random_access_iterator_tag> : public valid_iterator_tag_res<true, random_access_iterator_tag> {};

	template <>
	struct is_input_iterator_tagged<contiguous_iterator_tag> : public valid_iterator_tag_res<true, contiguous_iterator_tag> {};

	template <>
	struct is_input_iterator_tagged<bidirectional_iterator_tag> : public valid_iterator_tag_res<true, bidirectional_iterator_tag> {};

	template <>
	struct is_input_iterator_tagged<forward_iterator_tag> : public valid_iterator_tag_res<true, forward_iterator_tag> {};
	
	template <>
	struct is_input_iterator_tagged<input_iterator_tag> : public valid_iterator_tag_res<true, input_iterator_tag> {};

	template <>
	struct is_input_iterator_tagged<output_iterator_tag> : public valid_iterator_tag_res<true, output_iterator_tag> {};

	template <typename T>
	struct is_ft_iterator_tagged : public valid_iterator_tag_res<false, T> {};

	template <>
	struct is_ft_iterator_tagged<random_access_iterator_tag> : public valid_iterator_tag_res<true, random_access_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged< ::std::random_access_iterator_tag> : public valid_iterator_tag_res<true, ::std::random_access_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged<bidirectional_iterator_tag> : public valid_iterator_tag_res<true, bidirectional_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged< ::std::bidirectional_iterator_tag> : public valid_iterator_tag_res<true, ::std::bidirectional_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged<forward_iterator_tag> : public valid_iterator_tag_res<true, forward_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged< ::std::forward_iterator_tag> : public valid_iterator_tag_res<true, ::std::forward_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged<input_iterator_tag> : public valid_iterator_tag_res<true, input_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged< ::std::input_iterator_tag> : public valid_iterator_tag_res<true, ::std::input_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged<output_iterator_tag> : public valid_iterator_tag_res<true, output_iterator_tag> {};

	template <>
	struct is_ft_iterator_tagged< ::std::output_iterator_tag> : public valid_iterator_tag_res<true, ::std::output_iterator_tag> {};

	template <typename T>
	class InvalidIteratorException : public std::exception
	{
		private:
			std::string	_msg;
		public:
			InvalidIteratorException() throw() { _msg = "Is invalid iterator tag : " + std::string(typeid(T).name()); }
			InvalidIteratorException(const InvalidIteratorException&) throw() {};
			InvalidIteratorException&	operator=(const InvalidIteratorException&) throw() {};
			virtual ~InvalidIteratorException() throw() {};
			virtual const char * what() const throw() { return (_msg.c_str()); }
	};

	template <bool Cond, class T = void>
	struct enable_if {};

	template <class T>
	struct enable_if<true, T> { typedef T type; };

	template <class InputIterator1, class InputIterator2>
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1) return false;
			else if (*first1 < *first2) return true;
			first1++;
			first2++;
		}
		return (first2 != last2);
	}

	template <class InputIterator1, class InputIterator2, class Compare>
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1)) return false;
			else if (comp(*first1, *first2)) return true;
			first1++;
			first2++;
		}
		return (first2 != last2);
	}

	template <class InputIterator1, class InputIterator2>
	bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1)
		{
			if (*first1++ != *first2++) return false;
		}
		return (true);
	}

	template <class T1, class T2>
	struct pair
	{
		typedef T1	first_type;
		typedef T2	second_type;
		first_type	first;
		second_type	second;
		pair() : first(), second() {}
		pair(first_type const & x, second_type const & y) : first(x), second(y) {}
		template <class U1, class U2>
		pair(pair<U1, U2> const & p) : first(p.first), second(p.second) {}
		template <class U1, class U2>
		pair<U1, U2>&	operator=(pair<U1, U2> const & rhs)
		{
			if (this == &rhs)
				return (*this);
			first = rhs.first;
			second = rhs.second;
			return (*this);
		}
		~pair() {};
	};

	template <class T1, class T2>
	bool	operator==(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		if (lhs.first == rhs.first && lhs.second == rhs.second) return (true);
		return (false); 
	};

	template <class T1, class T2>
	bool	operator!=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (!(lhs == rhs));
	};

	template <class T1, class T2>
	bool	operator<(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		if (lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second)) return (true);
		return (false); 
	};

	template <class T1, class T2>
	bool	operator<=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		if (lhs.first <= rhs.first || (lhs.first == rhs.first && lhs.second <= rhs.second)) return (true);
		return (false); 
	};

	template <class T1, class T2>
	bool	operator>(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		if (lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second)) return (true);
		return (false); 
	};

	template <class T1, class T2>
	bool	operator>=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		if (lhs.first >= rhs.first || (lhs.first == rhs.first && lhs.second >= rhs.second)) return (true);
		return (false); 
	};

	template <class T1, class T2>
	pair<T1, T2>	make_pair(T1 t, T2 u)
	{
		return (pair<T1, T2>(t, u));
	}
}
