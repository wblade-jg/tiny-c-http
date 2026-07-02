#ifndef RADIXTREEMAP_H
#define RADIXTREEMAP_H

#include "list.h"
#include <stdbool.h>

typedef struct radixnode{
  char* prefix;
  void* value;
  ArrayList* children;
  bool isTerminal;
}RadixNode;

typedef struct radix{
  RadixNode* root;
}RadixTree;

RadixTree* createRadixTree();
void radixInsert(void* m, char* key, void* value);
void* radixGet(void* m, char* key);

#endif
