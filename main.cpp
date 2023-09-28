//
// Created by lilei on 23-9-18.
//
#include "Vector.h"
#include <cstdio>
#include <memory>

template <class T> void print_vector(T &a) {
  for (int i = 0; i < a.size(); ++i) {
    std::printf("%d ", *(a.vb.elem + i));
  }
  printf("\n");
}

int main() {

  vector<int> vec(5, 1);

  vector<int> vec2(5, 2);

  vector<int> vec3 = vec2;

  std::swap(vec, vec2);
  print_vector(vec);
  print_vector(vec2);



  std::swap(vec, vec3);
  print_vector(vec);
  print_vector(vec3);

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(2);
  vec.push_back(2);
  vec.push_back(2);
  print_vector(vec);

  printf("\n");


  vector<int> vec4 = {1, 2, 3, 4};
  print_vector(vec4);
}
