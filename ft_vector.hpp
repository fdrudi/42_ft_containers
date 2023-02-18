/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 19:28:01 by ddelladi          #+#    #+#             */
/*   Updated: 2023/02/18 13:28:16 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <stdexcept>
#include <iostream>
#include "utility.hpp"
#include "iterator.hpp"

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
	public:
		typedef T														value_type;
		typedef Alloc													allocator_type;
		typedef typename allocator_type::reference						reference;
		typedef typename allocator_type::const_reference				const_reference;
		typedef typename allocator_type::pointer						pointer;
		typedef typename allocator_type::const_pointer					const_pointer;
		typedef typename allocator_type::size_type						size_type;
		typedef ft::random_access_iterator<value_type>					iterator;
		typedef ft::random_access_iterator<const value_type>			const_iterator;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;

		// Constructor

		explicit vector(const allocator_type &alloc = allocator_type()) :
		_size(0),
		_capacity(0),
		_begin(NULL),
		_end(NULL),
		_alloc(alloc)
		{}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type()) :
		_size(0),
		_capacity(0),
		_begin(NULL),
		_alloc(alloc)
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
		}

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
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = 0) : _alloc(alloc)
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
		}

		vector(const vector &x)
		{
			_size = x.size();
			_capacity = x.capacity();
			_begin = _alloc.allocate(x.capacity());
			if (!x.empty())
				assign(x.begin(), x.end());
			_end = _begin + _size;
		}

		~vector()
		{
			clear();
			if (this->_begin != NULL || _capacity != 0)
				_alloc.deallocate(_begin, _capacity);
		}

		// Member functions

		void assign(size_type count, const T &value)
		{
			this->clear();
			if (count > this->capacity())
				this->reserve(count);
			this->insert(this->begin(), count, value);
		};

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = 0)
		{
			bool is_valid;

			is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value;
			if (!is_valid)
				throw ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::type>();
			this->clear();
			size_type count = ft::distance(first, last);
			if (count > this->capacity())
				this->reserve(count);
			this->insert(this->begin(), first, last);
		};

		void reserve(size_type n)
		{
			pointer 	prev_begin = _begin;
			pointer 	prev_end = _end;
			size_type 	prev_capacity = _capacity;
			pointer		tmp_begin = prev_begin;

			if (n > max_size())
				throw std::length_error("vector::reserve");
			if (n > _capacity)
			{
				_begin = _alloc.allocate(n);
				_end = _begin;
				_capacity = n;
				while (prev_begin != prev_end)
				{
					_alloc.construct(_end, *prev_begin);
					_end++;
					prev_begin++;
				}
				_end = _begin + ft::distance(tmp_begin, prev_end);
				if (tmp_begin)
					_alloc.deallocate(tmp_begin, prev_capacity);
			}
		};

		void resize(size_type n, value_type val = value_type())
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

		void push_back(const value_type &val)
		{
			if (_size == _capacity && _capacity + 1 > this->max_size())
				throw std::length_error("vector::push_back");
			if (this->empty())
				this->reserve(1);
			else if (_size == _capacity)
				this->reserve(_capacity * 2);
			_alloc.construct(_end++, val);
			_size++;
		}

		void pop_back()
		{
			if (this->empty())
				return;
			iterator iter = this->end() - 1;
			_alloc.destroy(&(*iter));
			_size--;
			_end--;
		}

		iterator insert(iterator position, const value_type &val)
		{
			size_type	dist = position - this->begin();
			if (this->size() == this->capacity())
			{
				if (!this->capacity())
					this->reserve(1);
				else
					this->reserve(this->size() * 2);
				position = this->begin() + dist;
			}
			_end++;
			size_type	idx = _size + 1;
			size_type	idx2 = _size;
			dist = this->end() - position + 1;
			while (dist--)
				_begin[idx--] = _begin[idx2--];
			*position = val;
			_size++;
			return (position);
		}

		void insert(iterator position, size_type n, const value_type &val)
		{
			size_type	dist = position - this->begin();
			size_type	finalSize = this->size() + n;
			if (finalSize > this->capacity())
				this->reserve(finalSize);
			position = this->begin() + dist;
			_end = _begin + _size;
			size_type	toMove = ft::distance(position, this->end());
			_end = _begin + finalSize;
			_size = finalSize;
			int i = 1;
			while (toMove--)
				_alloc.construct(_end - i++, _begin[dist + toMove]);
			while (n--)
				_alloc.construct(_begin + dist++, val);
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = 0)
		{
			bool is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value;
			if (!is_valid)
				throw ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::type>();

			size_type	positionDist = position - this->begin();
			size_type	finalSize = this->size() + ft::distance(first, last);
			size_type	new_cap = this->capacity();
			if (finalSize > this->capacity())
			{
				if (!this->capacity())
					new_cap = 1;
				while (finalSize > new_cap)
					new_cap *= 2;
				this->reserve(new_cap);
			}
			position = this->begin() + positionDist;
			_end = _begin + _size;
			size_type	toMove = ft::distance(position, this->end());
			_end = _begin + finalSize;
			_size = finalSize;
			size_type	dist = ft::distance(first, last);
			try
			{
				int i = 1;
				while (toMove--)
					_begin[_size - i++] = _begin[positionDist + toMove];
				while (dist--)
					_begin[positionDist++] = *first++;
			}
			catch (...)
			{
				_size = 0;
				_capacity = 0;
				throw "hey";
			}
		}

		iterator erase(iterator first, iterator last)
		{
			if (last == this->end())
			{
				_size = first - _begin;
				_end = _begin + _size;
				size_type	dist = ft::distance(first, last);
				size_type	idx = last - _begin;
				while (dist--)
				{
					_alloc.destroy(_begin + idx);
					idx--;
				}
				return (this->end());
			}
			else
			{
				iterator	ret = first;
				_size -= ft::distance(first, last);
				while (last != this->end())
				{
					_alloc.construct(first.pointed(), *last++);
					first++;
				}
				_end = _begin + _size;
				return (ret);
			}
		}

		iterator erase(iterator position)
		{
			iterator	ret = position;
			size_type	idx = position - this->begin();
			size_type	dist = this->end() - position;
			while (dist-- && idx + 1 < this->size())
			{
				_begin[idx] = _begin[idx + 1];
				idx++;
			}
			_alloc.destroy(_end--);
			_size--;
			return (ret);
		}

		void swap(vector &x)
		{
			pointer					tmpPtrBgn = x._begin;
			pointer					tmpPtrEnd = x._end;
			size_type				tmpCapacity = x._capacity;
			size_type				tmpSize = x._size;
			allocator_type			tmpAlloc = x._alloc;

			if (x == *this)
				return ;
			x._begin = this->_begin;
			x._capacity = this->_capacity;
			x._alloc = this->_alloc;
			x._size = this->_size;
			x._end = this->_end;
			this->_begin = tmpPtrBgn;
			this->_capacity = tmpCapacity;
			this->_alloc = tmpAlloc;
			this->_size = tmpSize;
			this->_end = tmpPtrEnd;
		}

		void clear()
		{
			while (_size)
			{
				_alloc.destroy(_end);
				_end--;
				_size--;
			}
		}

		// Element access

		reference		at(size_type pos)
		{
			if (pos >= _size)
				throw std::out_of_range("vector::out_of_range");
			return (*(_begin + pos));
		};

		const_reference	at(size_type pos) const
		{
			if (pos >= _size)
				throw std::out_of_range("vector::out_of_range");
			return (*(_begin + pos));
		};

		reference		operator[](size_type pos)
		{
			if (pos >= _capacity)
				throw std::out_of_range("vector::out_of_range");
			return (*(_begin + pos));
		};

		const_reference	operator[](size_type pos) const
		{
			if (pos >= _capacity)
				throw std::out_of_range("vector::out_of_range");
			return (*(_begin + pos));
		};

		reference		front() { return (*_begin); };
		const_reference front() const { return (*_begin); };
		reference		back() { return (*(_end - 1)); };
		const_reference back() const { return (*(_end - 1)); };
		T*				data() { return (_begin); };
		const T*		data() const { return (_begin); };

		// Iterators

		iterator				begin() { return (_begin); };
		const_iterator			begin() const { return (_begin); };
		iterator				end() { return (_end); };
		const_iterator			end() const { return (_end); };
		reverse_iterator		rbegin() { return reverse_iterator(this->end()); };
		const_reverse_iterator	rbegin() const { return reverse_iterator(this->end()); };
		reverse_iterator		rend() { return reverse_iterator(this->begin()); };
		const_reverse_iterator	rend() const { return reverse_iterator(this->begin()); };

		// Vector info

		bool			empty() const { return (size() == 0 ? true : false); };
		size_type		size() const { return (_size); };
		size_type		max_size() const { return (allocator_type().max_size()); };
		size_type		capacity() const { return (_capacity); };
		allocator_type	get_allocator() const { return (_alloc); };

	private:
		size_type		_size;
		size_type		_capacity;
		pointer			_begin;
		pointer			_end;
		allocator_type	_alloc;
	};

	// Overloads
	template <class T, class Alloc>
	bool operator==(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename vector<T>::const_iterator iter = lhs.begin();
		typename vector<T>::const_iterator iter2 = rhs.begin();
		while (iter != lhs.end())
		{
			if (iter2 == rhs.end() || *iter != *iter2)
				return (false);
			iter++;
			iter2++;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!=(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class T, class Alloc>
	bool operator<(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Alloc>
	bool operator<=(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <class T, class Alloc>
	bool operator>(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class Alloc>
	bool operator>=(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(rhs > lhs));
	}

	// template <class T, class Alloc>
	// void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
	// {
	// 	x.swap(y);
	// }
}
namespace std
{
	template <class T, class Alloc>
	void	swap(ft::vector<T, Alloc>& x, ft::vector<T, Alloc>& y)
	{
		x.swap(y);
	}
}
