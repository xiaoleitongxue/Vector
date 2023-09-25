//
// Created by lilei on 23-9-18.
//

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <memory>

template<class T, class A = std::allocator<T>>
struct vector_base
{
    A alloc;
    T* elem;
    T* space;
    T* last;

    vector_base(const A& a, typename A::size_type n):alloc{a}, elem{alloc.allocate(n)}, space{elem + n}, last{elem + n}{}

    ~vector_base(){
        alloc.deallocate(elem, last-elem);
    }

    vector_base(const vector_base&) = delete;
    vector_base& operatr=(const vector_base&) = delete;
    vector_base(vector_base&&);
    vector_base& operator=(vector_base&&);
};





template <class T, class A = std::allocator<T>>
class vector
{
    vector_base<T, A> vb;
    void destroy_elements();

public:
    using size_type = unsigned int;
    explicit vector(size_type n, const T &val = T(), const A & = A());
    
    vector(const vector &a);
    vector &operator=(const vector &a);

    vector(vector &&a);
    vector &operator=(vector &&a);

    ~vector();

    size_type size() const { return space - elem; }
    size_type capaciity() const { return last - elem; }

    void reserve(size_type n);

    void resize(size_type n, const T & = {});
    void clear(){resize(0);}
    void push_back(const T &);
};

template <class T, class A>
inline void vector<T, A>::destroy_elements()
{
    for(T* p = vb.elem; p != vb.space; ++p){
        p->~T();
    }
    vb.space = vb.elem;
}

template <class T, class A>
inline vector<T, A>::vector(size_type n, const T &val, const A &a) : alloc{a}
{
    elem = alloc.allocate(n);
    T *p;
    try
    {
        T *end = elem + n;
        for (p = elem; p != end; ++p)
        {
            alloc.construct(p, val);
        }
        last = space = p;
    }
    catch (...)
    {
        for (T *q = elem; q != p; ++q)
        {
            alloc.destroy(q);
        }
        alloc.deallocate(elem, n);
        throw;
    }
}

#endif // VECTOR_VECTOR_H