//
// Created by lilei on 23-9-18.
//
#include "Vector.h"
#include <stdio.h>

int main(){
    // Vector vec = {1, 2, 3};
    // printf("%d\n", vec.capacity());
    Vector<int> vec(10);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    printf("%d\n", vec.capacity());
    for(int i = 0; i < vec.size(); ++i){
        printf("%d ", vec[i]);
    }


    printf("\n");

}
