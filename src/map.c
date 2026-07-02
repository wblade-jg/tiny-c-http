#include <stdlib.h>
#include "map.h"
#include <string.h>

typedef struct node{
  char* key;
  void* value;
  struct node* next;
} Node;

typedef struct hashmap{
  int capacity;
  int size;
  Node** storage;
  float loadFactor;
} Hashmap;

unsigned long hash(unsigned char *str) {
   unsigned long hash = 5381;
   int c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

Hashmap* createHashmap(int capacity){
  Hashmap* map = malloc(sizeof(Hashmap));
  map->capacity = capacity;
  map->storage = calloc(capacity, sizeof(Node*));
  return map;
}

void resize(void* map){
  Hashmap* m = (Hashmap *)map;
  int new_capacity = m->capacity*2;
  free(m->storage);
  m->storage = calloc(new_capacity, sizeof(Node*));
  m->capacity = new_capacity;
  
  for(int i = 0; i < m->capacity; i++){
    Node* current = m->storage[i];
    if(current!= NULL){
      int index = (int)(hash((unsigned char*)current->key) % m->capacity);
      m->storage[index] = current;
    }
  }
}

void insert(void* map, char* key, void* value ){
    Hashmap* m = (Hashmap *)map;
    int index = (int)(hash((unsigned char*)key) % m->capacity);
    Node* new_node = malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = m->storage[index];
    m->storage[index] = new_node;
    if(((++m->size)/m->capacity) >= m->loadFactor){
      resize(m);
    }
}

void* get(void* map, char* key){
  Hashmap* m = (Hashmap *)map;
  int index = (int)(hash((unsigned char*)key) % m->capacity);
  Node* current = m->storage[index];
  while(current!=NULL){
    if(strcmp(current->key, key) == 0) return current->value;
    current = current->next;
  }
  return NULL;
}

