//
// Created by lilei on 23-9-18.
//

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <algorithm>
#include <cstdio>
#include <memory>
#include <type_traits>
#include <utility>
#include <iostream>
template <class T, class A = std::allocator<T>> struct vector_base {
  A alloc;
  T *elem;
  T *space;
  T *last;

  vector_base(const A &a, typename A::size_type n)
      : alloc{a}, elem{alloc.allocate(n)}, space{elem + n}, last{elem + n} {}

  ~vector_base() { alloc.deallocate(elem, last - elem); }

  vector_base(const vector_base &) = delete;
  vector_base &operator=(const vector_base &) = delete;
  vector_base(vector_base &&);
  vector_base &operator=(vector_base &&);
};

template <class T, class A>
inline vector_base<T, A>::vector_base(vector_base &&a)
    : alloc{a.alloc}, elem{a.elem}, space{a.space}, last{a.last} {
  a.elem = a.space = a.last = nullptr;
}

template <class T, class A>
inline vector_base<T, A> &vector_base<T, A>::operator=(vector_base &&a) {
  if (this != &a) {
    // release current object's resources
    alloc.deallocate(elem, last - elem);
    alloc = a.alloc;
    elem = a.elem;
    space = a.space;
    last = a.last;

    a.elem = nullptr;
    a.space = nullptr;
    a.last = nullptr;
  }
  return *this;
}

template <class T, class A = std::allocator<T>> class vector {
public:
  vector_base<T, A> vb;
  void destroy_elements();
  void destroy(T *b, T *e);

public:
  using size_type = unsigned int;
  explicit vector(size_type n, const T &val = T(), const A & = A());

  vector(const vector &a);
  vector &operator=(const vector &a);

  vector(vector &&a);
  vector &operator=(vector &&a);

  ~vector(){};

  size_type size() const { return vb.space - vb.elem; }
  size_type capaciity() const { return vb.last - vb.elem; }

  void reserve(size_type n);

  void resize(size_type n, const T & = {});
  void clear() { resize(0); }
  void push_back(const T &);
};

template <class T, class A>
vector<T, A>::vector(vector &&a) : vb{std::move(a.vb)} {

}

template <class T, class A> vector<T, A> &vector<T, A>::operator=(vector &&a) {
  // clear();
  if (this != &a) {
    // release current object's resources
    vb = std::move(a.vb);
  }
  return *this;
}

template <class T, class A> inline void vector<T, A>::destroy_elements() {
  for (T *p = vb.elem; p != vb.space; ++p) {
    p->~T();
  }
  vb.space = vb.elem;
}

template <class T, class A>
vector<T, A>::vector(size_type n, const T &val, const A &a) : vb{a, n} {

  T *p;
  try {
    T *end = vb.elem + n;
    for (p = vb.elem; p != end; ++p) {
      vb.alloc.construct(p, val);
    }
    vb.last = vb.space = p;
  } catch (...) {
    for (T *q = vb.elem; q != p; ++q) {
      vb.alloc.destroy(q);
    }
    vb.alloc.deallocate(vb.elem, n);
    throw;
  }
}

template <class T, class A> void vector<T, A>::reserve(size_type newalloc) {
  if (newalloc <= capaciity())
    return;
  vector_base<T, A> b(vb.alloc, capaciity());

  // move vb to b
  std::copy(vb.elem, vb.last, b.elem);
  vb = std::move(b);
}// release b 

template <class T, class A> void vector<T, A>::destroy(T *b, T *e) {
  for (; b != e; ++b) {
    b->~T();
  }
}
template <class T, class A>
void vector<T, A>::resize(size_type newsize, const T &val) {
  reserve(newsize);
  if (size() < newsize) {
    std::uninitialized_fill(vb.elem + size(), vb.elem + newsize, val);
  } else {
    destroy(vb.elem + size(), vb.elem + newsize);
  }

  vb.space = vb.last = vb.elem + newsize;
}

template <class T, class A> void vector<T, A>::push_back(const T &x) {
  if (capaciity() == size())
    reserve(size() ? 2 * size() : 8);
  vb.alloc.construct(vb.elem + size(), x);
  ++vb.space;
}

// copy assignment
template <class T, class A>
vector<T, A> &vector<T, A>::operator=(const vector &a) {
  vector temp{a}; // copy constructor
  std::swap(*this, temp);
  return *this;
} // temp will be released

// copy constructor
template <class T, class A>
vector<T, A>::vector(const vector &a) : vb{a.vb.alloc, a.size()} {
  T *p, *r;
  try {
    T *end = vb.elem + a.size();
    for (p = vb.elem, r = a.vb.elem; p != end; ++p, ++r) {
      vb.alloc.construct(p, *r);
    }
    vb.last = vb.space = p;
  } catch (...) {
    for (T *q = vb.elem; q != p; ++q) {
      vb.alloc.destroy(q);
    }
    vb.alloc.deallocate(vb.elem, a.size());
    throw;
  }
}

#endif // VECTOR_VECTOR_H
