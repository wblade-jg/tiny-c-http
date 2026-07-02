#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

ArrayList* createArrayList(int size){
  ArrayList* list = malloc(sizeof(ArrayList));
  list->elements = calloc(size, sizeof(void*));
  list->capacity = size;
  list->length = 0;
  return list;
}

void* getAt(ArrayList* list, int index){
  return list->elements[index];
}

void resize(ArrayList* list){
  free(list->elements);
  const int newSize = list->capacity*2;
  list->elements = calloc(newSize, sizeof(void*));
  list->capacity = newSize;
}

void insertAt(ArrayList* list, int index, void* value){
  if(index<list->capacity && index >= 0){ 
    list->elements[index] = value;
    list->length++;
  }
}

void push(ArrayList* list, void* value){
  if(list->length >= list->capacity){
    resize(list);
  }
  insertAt(list, list->length, value);
}
