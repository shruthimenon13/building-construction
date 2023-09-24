#ifndef ___COMMON_H___
#define ___COMMON_H___

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <climits>
#include <cstddef>
#include <queue>
#include <iomanip>

using namespace std;

class Red_black_node;

enum COLOR { RED, BLACK };


typedef struct {
  int building_num;
  int execution_time;
  int total_time;
  Red_black_node* rbt_node_ptr;
}MinHeap_t;

typedef struct {
  int building_num;
  int execution_time;
  int total_time;
  MinHeap_t* minheap_node_ptr;
}RBT_t;



#include "redblack_node.cpp"



#define MAX_HEAP_SIZE 2000

#endif
