//
// Created by lilei on 23-9-18.
//

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <initializer_list>
#include <memory>
template <typename T>
class Vector
{
public:
    explicit Vector(int s);

    Vector();

    Vector(std::initializer_list<T>);

    void push_back(const T &v);

    void push_back(const T &&v);

    ~Vector() { delete[] elem; }

    T &operator[](int i);

    [[nodiscard]] int size() const;

    [[nodiscard]] int capacity() const;

    void reserve(int new_sz);

    void construct_at(T *space_, const T &t);
    void construct_at(T *space_, const T &&t);

    // copy constructor
    Vector(const Vector &);
    // copy assignment
    Vector& operator=(const Vector &);

    

private:
    T *elem;  // pointer to first element
    T *space; // pointer to first unused slot
    T *last;  // pointer to last slot
    int sz;
    std::allocator<T> alloc;
};

#include <algorithm>
template <typename T>
Vector<T>::Vector(std::initializer_list<T> lst) : sz{0}
{
    alloc = std::allocator<T>();
    reserve(3);
    elem = space;
    // construct
    for (T i : lst)
    {
        construct_at(space, i);
        ++space;
    }
    sz = static_cast<int>(lst.size());
}
template <typename T>
Vector<T>::Vector() : alloc{std::allocator<T>()}, elem{nullptr}, sz{0}
{
    reserve(8);
    elem = nullptr;
}
template <typename T>
Vector<T>::Vector(int s) : alloc{std::allocator<T>()}
{
    reserve(s);
    elem = space;
    // construct
    for (int i = 0; i != s; ++i)
    {
        construct_at(space, 0);
        ++space;
    }
    sz = s;
}
template <typename T>
void Vector<T>::push_back(const T &d)
{
    if (capacity() <= size())
    {
        reserve(size() == 0 ? 8 : 2 * size());
        construct_at(space, d);
        ++space;
    }
    else
    {
        construct_at(space, d);
        ++space;
    }
    sz++;
}
template <typename T>
void Vector<T>::push_back(const T &&d)
{
    if (capacity() <= size())
    {
        reserve(size() == 0 ? 8 : 2 * size());
        construct_at(space, d);
        ++space;
    }
    else
    {
        construct_at(space, d);
        ++space;
    }
    sz++;
}
template <typename T>
int Vector<T>::size() const
{
    return sz;
}
template <typename T>
int Vector<T>::capacity() const
{
    return (last - elem) / sizeof(T);
}
template <typename T>
T &Vector<T>::operator[](int i)
{
    T *p = elem;
    int j = 0;
    while (j < i)
    {
        ++j;
        p++;
    }
    return *p;
}
template <typename T>
void Vector<T>::reserve(int new_sz)
{
    T *p = alloc.allocate(new_sz);
    if (sz > 0)
    {
        T *new_spaces = p;
        for (int i = 0; i < sz; ++i)
        {
            construct_at(new_spaces, *(elem + i));
            new_spaces++;
        }
        alloc.deallocate(elem, capacity());
        elem = p;
        space = new_spaces;
        last = p + new_sz * sizeof(T);
    }
    else
    {
        space = p;
        last = p + new_sz * sizeof(T);
    }
}
template <typename T>
void Vector<T>::construct_at(T *space_, const T &t)
{
    alloc.construct(space_, t);
}

template <typename T>
void Vector<T>::construct_at(T *space_, const T &&t)
{
    alloc.construct(space_, t);
}

template <typename T>
Vector<T>::Vector(const Vector &v):sz{v.size()}
{
    alloc = std::allocator<T>();
    reserve(v.size());
    elem = space;
    // construct
//    for (T & i : v)
//    {
//        construct_at(space, i);
//        ++space;
//    }

}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector &v)
{

    return v;
}

#endif // VECTOR_VECTOR_H
