#ifndef __VECTOR_HPP__
# define __VECTOR_HPP__

# include "iostream"

// FIXME
// # include "memory" // home::deletable
// # include "stdexcept" // home::deletable

# include "algorithm"
# include "iterator_reverse_iterator.hpp"
# include "util_lexicographical_compare.hpp"
# include "util_equal.hpp"
# include "util_swap.hpp"
# include "util_enable_if.hpp"
# include "util_is_integral.hpp"


namespace ft
{
	template<class T, class Allocator = std::allocator<T> >
	class vector
	{

	public:
		// types:
		typedef typename Allocator::reference	reference;
		typedef typename Allocator::pointer	pointer;

		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::const_pointer	const_pointer;

		typedef Allocator	allocator_type;
		typedef T		value_type;
		typedef size_t		size_type;

		typedef ptrdiff_t	difference_type;
		// typedef int		difference_type; // int works just as fine

		typedef pointer			iterator;
		typedef const_pointer	const_iterator;

		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;


	private:
		size_type		m_size;
		size_type		m_capacity;
		value_type		* m_vector;
		allocator_type		m_allocator;

		size_type	m_next_size(size_type n)
		{
			size_type	res;

			res = n > max_size() / 2 ? max_size() : m_capacity * 2;
			if (!res)
				res = 1;
			if (res < n)
				return (n);
			return (res);
		}
	public:

		// default
		explicit vector(const allocator_type & alloc = allocator_type())
		{
			m_size = 0;
			m_capacity = 0;
			m_allocator = alloc;
			m_vector = m_allocator.allocate(0);
		}


		// fill
		explicit vector(
			size_type n,
			const T & value = T(),
			const allocator_type& alloc = allocator_type()
		)
		{
			m_size = n;
			m_capacity = n;
			m_allocator = alloc;
			m_vector = m_allocator.allocate(n);

			for (size_type i = 0; i < m_size; i++)
				m_allocator.construct(m_vector + i, value);
		}

		// FIXME - mar 8 :: failed mazoise copy-swap test @ 381c381
		// FIXME - mar 9 :: corr. failed mazoise copy-swap @ Segfault
		// FIXED - mar 10 :: Fixed mazoise copy-swap test @ 381c381

		// range
		template<class InputIterator>
		vector(InputIterator first, InputIterator last,
			const allocator_type & alloc = allocator_type(),
			typename ft::enable_if<!ft::is_integral<InputIterator>::value,
				InputIterator>::type* = 0
		) : m_size(0), m_capacity(0), m_allocator(alloc)
		{
			m_vector = m_allocator.allocate(0);
			assign(first, last);
			// Fixed by this line
			m_capacity = std::distance(first, last);
		}


		vector(const vector & val)
		{
			m_allocator = val.m_allocator;
			m_size = val.m_size;
			m_capacity = val.m_size ;
			m_vector = m_allocator.allocate(m_capacity);

			for (size_type i = 0; i < m_size; i++)
			{
				m_allocator.construct(m_vector + i, *(val.m_vector + i));
			}
		}

		~vector()
		{
			clear();
			m_allocator.deallocate(m_vector, m_capacity);
		}

		vector & operator = (const vector & dummy)
		{
			if (*this == dummy)
				return *this;

			assign(dummy.begin(), dummy.end());

			return *this;
		}

		template<class InputIterator>
		void	assign(
			InputIterator first,
			InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value,
				InputIterator>::type* = 0
		) // Check if is_integral, if yes, it's not an Iterator
		{
			clear();
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}

		void	assign(size_type n, const T & val)
		{
			size_type	i;

			clear();
			i = -1;
			while (++i < n)
			{
				push_back(val);
			}
		}


		// method : get_allocator

		allocator_type get_allocator() const
		{
			return m_allocator;
		}


		// iterators:

		iterator	begin()
		{
			return iterator(m_vector);
		}

		const_iterator	begin() const
		{
			return const_iterator(m_vector);
		}

		iterator	end()
		{
			return iterator(m_vector + size());
		}

		const_iterator	end() const
		{
			return const_iterator(m_vector + size());
		}

		reverse_iterator	rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator	rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator	rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator	rend() const
		{
			return const_reverse_iterator(begin());
		}


		// capacity:

		size_type size() const
		{
			return m_size;
		}

		size_type	max_size() const
		{
			return m_allocator.max_size();
		}

		void	resize(size_type n, T c = T())
		{
			// FIXME - mar 9 :: kk's soln, passed resize test @ 25c25
			// FIXED - mar 8 :: mine, failed resize test @ 25c25
			// FIXED - mar 10 :: concise++ improvement

			if (n > size())
			{
				reserve(m_next_size(n));
				insert(end(), n - size(), c);
			}
			else if (n < size())
			{
				erase(begin() + n, end());
			}

			

		}

		size_type capacity() const
		{
			return (m_capacity);
		}

		bool	empty() const
		{
			if (m_size != 0)
				return false;
			return true;
		}

		void	reserve(size_type n)
		{
			if (n > max_size())
			{
				throw(std::length_error("vector::reserve"));
			}

			if (n > capacity())
			{
				size_type	old_capacity = m_capacity;
				T		* tmp;

				m_capacity = n;
				tmp = m_allocator.allocate(m_capacity);

				for (size_type i = 0; i < m_size; i++)
				{
					m_allocator.construct(tmp + i, *(m_vector + i));
					m_allocator.destroy( & m_vector[i]);
				}

				m_allocator.deallocate(m_vector, old_capacity);
				m_vector = tmp;
			}
		}


		///	Accessors


		// []
		reference operator [] (size_type n)
		{
			return (*(m_vector + n));
		}

		const_reference operator [] (size_type n) const
		{
			return (*(m_vector + n));
		}


		// .at
		reference	at(size_type n)
		{
			if (n < 0 || n >= size())
				throw(std::out_of_range("vector::at"));
			return (*(m_vector + n));
		}

		const_reference	at(size_type n) const
		{
			if (n < 0 || n >= size())
				throw(std::out_of_range("vector::at const"));
			return (*(m_vector + n));
		}


		// .front & .back
		reference front()
		{
			return (*m_vector);
		}

		const_reference front() const
		{
			return (*m_vector);
		}

		reference back()
		{
			return (*(m_vector + (m_size - 1)));
		}

		const_reference back() const
		{
			return (*(m_vector + (m_size - 1)));
		}


		// modifiers:
		void push_back(const T & val)
		{
			if (m_capacity == 0)
				reserve(1);

			if (m_size == m_capacity)
				reserve(m_size * 2);

			T	*end = & m_vector[m_size];

			m_size++;
			m_allocator.construct(end, val);
		}

		void pop_back()
		{
			if (!empty())
			{
				m_size--;
				m_allocator.destroy( & m_vector[m_size]);
			}
		}


		iterator insert(iterator position, const T & val)
		{

			// std::cout << RED << __FUNCTION__ << " called 1 (single element)" << RESET nl2reset;

			size_type	diff = position - begin();

			if (m_size + 1 > m_capacity)
			{
				reserve(m_size > 0 ? m_size * 2 : 1);
			}
			for (size_type i = m_size; i > diff; i--)
			{
				m_allocator.construct( & m_vector[i], m_vector[i - 1]);
				m_allocator.destroy( & m_vector[i - 1]);
			}
			m_allocator.construct( & m_vector[diff], val);
			m_size++;

			return begin() + diff;
		}


		void insert(iterator position, size_type n, const T & val)
		{
			///	FIXME -  mar 8 :: fixing cf. mazoise Resize & Insert tests

			if (m_capacity < m_size + n)
			{
				size_type	diff;

				diff = position - begin();
				reserve(m_next_size(m_size + n));
				position = begin() + diff;
			}
			for (size_type i = 0; i < n; i++)
			{
				position = insert(position, val) + 1;
			}

		}

		template<class InputIterator>
		void	insert(
			iterator position,
			InputIterator first,
			InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value,
				InputIterator>::type* = 0
		)
		{
			difference_type	dist = 0;
			difference_type	diff;

			for (InputIterator tmp = last; tmp != first; tmp--)
				dist++;
			diff = position - begin();
			if (m_size + dist > m_capacity)
				reserve(m_size + std::max(m_size, (size_type) dist));

			for (difference_type i = m_size; i > diff; i--)
			{
				m_allocator.construct( & m_vector[i + dist - 1], m_vector[i - 1]);
				m_allocator.destroy( & m_vector[i - 1]);
			}

			for (difference_type i = 0; i < dist; i++)
			{
				m_allocator.construct( & m_vector[diff + i], *first);
				first++;
			}
			m_size += dist;
		}

		iterator erase(iterator position)
		{
			iterator it = position;
			iterator ite = end();
			iterator ret = it;

			if (position + 1 == ite)
			{
				pop_back();
			}
			else
			{
				for (; it + 1 != ite; it++)
				{
					m_allocator.destroy(it);
					m_allocator.construct(it, *(it + 1));
				}
				m_allocator.destroy(it);
				m_size--;
			}
			return ret;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator it = first;
			iterator ret;
			for (; first != last; ++first)
				ret = erase(it);

			return ret;
		}

		void	swap(vector & val)
		{
			if (*this == val)
				return ;
			ft::swap(m_size, val.m_size);
			ft::swap(m_capacity, val.m_capacity);
			ft::swap(m_vector, val.m_vector);
		}

		void	clear()
		{
			for (size_type i = 0; i < m_size; i++)
				m_allocator.destroy(m_vector + i);
			m_size = 0;
		}
	};

	template<class T, class Allocator>
	bool	operator == (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		typedef typename vector<T, Allocator>::const_iterator it_type;

		if (L.size() ^ R.size())
			return false;
		return ft::equal<it_type, it_type>(L.begin(), L.end(), R.begin());

	}

	template<class T, class Allocator>
	bool operator != (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		return (!(L == R));
	}

	template<class T, class Allocator>
	bool operator < (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		return (ft::lexicographical_compare(L.begin(), L.end(), R.begin(), R.end()));
	}

	template<class T, class Allocator>
	bool operator> (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		return (R < L);
	}

	template<class T, class Allocator>
	bool operator >= (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		return (!(L < R));
	}

	template<class T, class Allocator>
	bool operator <= (
		const vector<T, Allocator> & L, 
		const vector<T, Allocator> & R
	)
	{
		return (!(R < L));
	}

	template<class T, class Allocator>
	void swap(vector<T, Allocator> & L, vector<T, Allocator> & R)
	{
		L.swap(R);
	}
}

#endif
