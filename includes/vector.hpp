#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include "Utils/RandomAccessIterator.hpp"

namespace ft {

    template<class T, class Alloc = std::allocator<T> >
    class vector {
        public:
            typedef T                                               value_type;
            typedef Alloc                                           allocator_type;
            typedef size_t                                          size_type;
            typedef ptrdiff_t                                       difference_type;
            typedef value_type&                                     reference;
            typedef const value_type&                               const_reference;
            typedef value_type*                pointer;
            typedef const value_type*    const_pointer;

            typedef RandomAccessIterator<value_type, reference, pointer>                    iterator;
            typedef RandomAccessIterator<value_type, const_reference,  const_pointer>        const_iterator;
            typedef ReverseRandomAccessIterator<value_type, reference,  pointer>             reverse_iterator;
            typedef ReverseRandomAccessIterator<value_type, const_reference,  const_pointer> const_reverse_iterator;
            
            


        private:
            pointer             ptr;
            allocator_type      alloc;
            size_type           len_size;
            size_type           cap;

        public:
            explicit    vector(const allocator_type &alloc = allocator_type());
            explicit    vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type());
            vector(iterator first, iterator last, const allocator_type &alloc = allocator_type());
            vector(const_iterator first, const_iterator last, const allocator_type &alloc = allocator_type());
            vector(const vector &x);
            ~vector();

            vector &operator=(const vector &x);

            //Iterators
			iterator				begin()			{ return iterator(this->ptr);}
			const_iterator			begin() const	{ return const_iterator(this->ptr);}
			iterator				end()			{ return iterator(&this->ptr[this->len_size]);}
			const_iterator			end() const		{ return const_iterator(&this->ptr[this->len_size]);}
			reverse_iterator		rbegin()		{ return reverse_iterator(&this->ptr[this->len_size - 1]);}
			const_reverse_iterator	rbegin() const	{ return reverse_iterator(&this->ptr[this->len_size - 1]);}
			reverse_iterator		rend()			{ return reverse_iterator(this->ptr - 1);}
			const_reverse_iterator	rend() const	{ return reverse_iterator(this->ptr - 1);}


            // --- Capacity ---

            size_type   size() const { return this->len_size; }
            size_type   max_size() const { return ( std::numeric_limits<size_type>::max() / (sizeof(value_type)));}
            size_type   capacity() const { return this->cap; }
            bool        empty() const { return !this->len_size; }

            void        resise(size_type n, value_type val = value_type());
            void        reserve(size_type n);

            // --- Element access ---

            reference           operator[](size_type n) { return this->ptr[n]; }
            const_reference     operator[](size_type n) const { return this->ptr[n]; }
            reference           front() { return this->ptr[0]; }
            const_reference     front() const { return this->ptr[0]; }
            reference           back() { return this->ptr[this->len_size - 1]; }
            const_reference     back() const { return this->ptr[this->len_size - 1]; }
            reference           at(size_type n)
            {
                if (n < this->len_size)
                    return this->ptr[n];
                throw std::out_of_range("vector");
            }

            const_reference     at(size_type n) const
            {
                if (n < this->len_size)
                    return this->ptr[n];
                throw std::out_of_range("vector");
            }


            // --- Modifiers ---

            void                assign(iterator first, iterator last);
            void                assign(const_iterator first, const_iterator last);
            void                assign(size_type n, const value_type &val);
            void                push_back(const value_type &val);
            void                pop_back();
            iterator            insert(iterator position, const value_type &val);
            void                insert(iterator position, size_type n, const value_type &val);
            void                insert(iterator position, iterator first, iterator last);
            void                insert(iterator position, const_iterator first, const_iterator last);
            iterator            erase(iterator position);
            iterator            erase(iterator first, iterator last);            
            void                swap(vector &x);
            void                clear();
    };

    template< typename T, typename Alloc >
    vector<T, Alloc>::vector(const allocator_type &alloc):
    ptr(NULL), alloc(alloc), len_size(0), cap(0) {}

    template< typename T, typename Alloc >
    vector<T, Alloc>::vector(size_type n, const value_type &val, const allocator_type &alloc):
    alloc(alloc), len_size(n), cap(n)
    {
        this->ptr = new value_type[n]();
        for (size_type i = 0; i < n; i++)
		    this->ptr[i] = val;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(iterator first, iterator last, const allocator_type &alloc):
    alloc(alloc), len_size(last - first), cap(last - first)
    {
        this->ptr = new value_type[this->cap];
        this->assign(first, last);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const_iterator first, const_iterator last, const allocator_type &alloc):
    alloc(alloc), len_size(last - first), cap(last - first)
    {
        this->ptr = new value_type[this->cap];
        this->assign(first, last);
    }

    template< typename T, typename Alloc >
    vector<T, Alloc>::vector(const vector &x):
    ptr(x.ptr), alloc(x.alloc), len_size(x.len_size), cap(x.cap) { *this = x; }

    template< typename T, typename Alloc >
    vector<T, Alloc>::~vector()
    {
        if (this->ptr)
        {
            delete[] this->ptr;
            this->ptr = NULL;
        }
    }

    template< typename T, typename Alloc >
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &x)
    {
        if (this->ptr)
        {
            delete[] this->ptr;
            this->ptr = NULL;
        }
        this->ptr = new value_type[x.cap]();
        this->cap = x.cap;
        this->len_size = x.len_size;
        this->assign(x.begin(), x.end());
        return *this;
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::resise(size_type n, value_type val)
    {
        while (n < this->len_size)
            this->pop_back();
        if (n > this->cap)
            this->reserve(n);
        while (n > this->len_size)
            this->push_back(val);
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::reserve(size_type n)
    {
        if (n <= this->cap)
            return;
        pointer temp = new value_type[n];
        for (size_type i = 0; i < this->len_size; i++)
            temp[i] = this->ptr[i];
        if (this->ptr)
        {
            delete []   this->ptr;
            this->ptr = NULL;
        }
        this->ptr = temp;
        this->cap = n;
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::assign(iterator first, iterator last)
    {
        this->clear();
        while (first != last)
        {
            this->push_back(*first);
            first++;
        }
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::assign(const_iterator first, const_iterator last)
    {
        this->clear();
        while (first != last)
        {
            this->push_back(*first);
            first++;
        }
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::assign(size_type n, const value_type &val)
    {
        this->clear();
        for (size_type i = 0; i < n; i++)
            this->push_back(val);
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::push_back(const value_type &val)
    {
        if (this->len_size == this->cap)
        {
            if (this->len_size == 0)
                this->reserve(2);
            else
                this->reserve(this->len_size * 2);
        }
        this->ptr[this->len_size] = val;
        this->len_size++;
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::pop_back()
    {
        if (this->len_size)
            this->len_size--;
    }

    template< typename T, typename Alloc >
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &val)
    {
        vector<T, Alloc> temp(position, this->end());
        this->len_size -= this->end() - position;
        this->push_back(val);
        iterator    it_begin = temp.begin();
        iterator    it_end = temp.end();
        while (it_begin != it_end)
        {
            this->push_back(*it_begin);
            it_begin++;
        }
    }

    template< typename T, typename Alloc >
    void vector<T, Alloc>::insert(iterator position, size_type n, const value_type &val)
    {
        vector<T, Alloc> temp(position, this->end());
        this->len_size -= this->end() - position;
        for (size_type i = 0; i < n; i++)
            this->push_back(val);
        iterator    it_begin = temp.begin();
        iterator    it_end = temp.end();
        while (it_begin != it_end)
        {
            this->push_back(*it_begin);
            it_begin++;
        }
    }

    template< typename T, typename Alloc >
    void vector<T, Alloc>::insert(iterator position, iterator first, iterator last)
    {
        vector<T, Alloc> temp(position, this->end());
        this->len_size -= this->end() - position;
        while (first != last)
        {
            this->push_back(*first);
            first++;
        }
        iterator    it_begin = temp.begin();
        iterator    it_end = temp.end();
        while (it_begin != it_end)
        {
            this->push_back(*it_begin);
            it_begin++;
        }
    }

    template< typename T, typename Alloc >
    void vector<T, Alloc>::insert(iterator position, const_iterator first, const_iterator last)
    {
        vector<T, Alloc> temp(position, this->end());
        this->len_size -= this->end() - position;
        while (first != last)
        {
            this->push_back(*first);
            first++;
        }
        iterator    it_begin = temp.begin();
        iterator    it_end = temp.end();
        while (it_begin != it_end)
        {
            this->push_back(*it_begin);
            it_begin++;
        }
    }

    template< typename T, typename Alloc >
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position)
    {
        iterator temp(position);
        iterator it_end = this->end() - 1;
        while (position != it_end)
        {
            *position = *(position + 1);
            position++;
        }
        this->len_size--;
        return temp;
    }

    template< typename T, typename Alloc >
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
    {
        iterator temp(last);
        iterator it_end = this->end();
        while (last != it_end)
        {
            *first = *last;
            first++;
            last++;
        }
        this->len_size -= (last - first);
        return temp;
    }

    template< typename T, typename Alloc >
    void    vector<T, Alloc>::swap(vector &x)
    {
        vector<T, Alloc> temp = *this;
        *this = x;
        x = temp;
    }

    template< typename T, typename Alloc >
    void vector<T, Alloc>::clear()
    {
        while (this->len_size)
            this->pop_back();
    }

    // Non-member function

    template<typename T, typename Alloc>
    bool    operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        typename ft::vector<T, Alloc>::const_iterator   begin_lhs = lhs.begin();
        typename ft::vector<T, Alloc>::const_iterator   begin_rhs = rhs.begin();
        typename ft::vector<T, Alloc>::const_iterator   end_lhs = lhs.end();
        typename ft::vector<T, Alloc>::const_iterator   end_rhs = rhs.end();

        while ((begin_lhs != end_lhs) && (begin_rhs != end_rhs))
        {
            if (*begin_lhs != *begin_rhs)
                return false;
            begin_lhs++;
            begin_rhs++;
        }
        if ((begin_lhs != end_lhs) && (begin_rhs != end_rhs))
            return false;
        return true;
    }

    template<typename T, typename Alloc>
    bool    operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        if (lhs == rhs)
            return false;
        return true;
    }

    template<typename T, typename Alloc>
    bool    operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        typename ft::vector<T, Alloc>::const_iterator   begin_lhs = lhs.begin();
        typename ft::vector<T, Alloc>::const_iterator   begin_rhs = rhs.begin();
        typename ft::vector<T, Alloc>::const_iterator   end_lhs = lhs.end();
        typename ft::vector<T, Alloc>::const_iterator   end_rhs = rhs.end();

        while (begin_lhs != end_lhs)
        {
            if ((begin_rhs != end_rhs) && (*begin_lhs > *begin_rhs))
                return false;
            else if (*begin_lhs < *begin_rhs)
                return true;
            begin_lhs++;
            begin_rhs++;
        }
        
        return (begin_rhs != end_rhs);
    }

    template<typename T, typename Alloc>
    bool    operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return (rhs < lhs);
    }

    template<typename T, typename Alloc>
    bool    operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template<typename T, typename Alloc>
    bool    operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

    template<typename T, typename Alloc>
    void    swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
    {
        vector<T, Alloc> temp = x;
        x = y;
        y = temp;
    }

}

#endif