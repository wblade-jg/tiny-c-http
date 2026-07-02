#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radixtreemap.h"

#define CHILDREN_SIZE 5

typedef RadixNode Node;

static Node* createNode(char* prefix, void* value, bool isTerminal){
  Node* node = malloc(sizeof(Node));
  node->prefix = strdup(prefix);
  node->value = value;
  node->isTerminal = isTerminal;
  node->children = createArrayList(CHILDREN_SIZE);
  return node;
}

RadixTree* createRadixTree(){
  RadixTree* radixTree = malloc(sizeof(RadixTree));
  radixTree->root = createNode("", NULL, 0);
  return radixTree;
}

int commonPrefixLen(char* s1, char* s2){
    int i = 0;
    while(s1[i] && s2[i] && s1[i] == s2[i]){
        i++;
    }
    return i;
}

void radixInsert(void* m, char* prefix, void* value){
  RadixTree* radixTree = (RadixTree *)m;
  int position = 0;
  int prefixLen = strlen(prefix);
  
  Node* current = radixTree->root;
  
  while(position < prefixLen){
    Node* match = NULL;
    for(int elementIndex = 0; elementIndex < current->children->length; elementIndex++){
      Node* element = getAt(current->children, elementIndex);
      if(element->prefix[0] == prefix[position]){
        match = element;
        break;
      }
    }

    if(match == NULL){
      push(current->children, createNode(prefix+position, value, 1));
      return;
    }
    
    int commonLen = commonPrefixLen(prefix+commonLen, match->prefix);

    if(commonLen < strlen(match->prefix)){
      Node* notCommonPrefix = createNode(match->prefix+commonLen, match->value, match->isTerminal);
      char* oldPrefix = match->prefix;
      match->prefix = strndup(match->prefix, commonLen);
      match->value = NULL;
      match->isTerminal = 0;
      free(oldPrefix);
      free(notCommonPrefix->children);
      notCommonPrefix->children = match->children;
      match->children = createArrayList(CHILDREN_SIZE);
      push(match->children, notCommonPrefix);
    }
    position+= commonLen;

    if(position == prefixLen){
      match->isTerminal = 1;
      match->value = value;
      return;
    }

    current = match;
    
  }
}

void iterate(RadixTree* radixTree, Node* node) {
  if(node == NULL) return;
  if(node->prefix && strlen(node->prefix) > 0) {
    printf("Prefix: %s %s\n", node->prefix, node->isTerminal ? "[TERMINAL]" : "");
  }
  if(node->children->length == 0) return;

  for(int i = 0; i < node->children->length; i++) {
    Node* child = (Node*)getAt(node->children, i);
    iterate(radixTree, child);
  }
}

void* radixGet(void* m, char* key){
  RadixTree* radixTree = (RadixTree *)m;
  int position = 0;
  int prefixLen = strlen(key);
  
  Node* current = radixTree->root;

  while(position < prefixLen){
    Node* match = NULL;
    int len = 0;
    for(int elementIndex = 0; elementIndex < current->children->length; elementIndex++){
      char* elementKey = ((Node *)getAt(current->children, elementIndex))->prefix;
      len = commonPrefixLen(key+position, elementKey);
      if(len == strlen(elementKey)){
        match = getAt(current->children, elementIndex);
        position+=len;
        break;
      }
    }

    if(match == NULL){
      return NULL;
    }

    current = match;
  }

  if(current->isTerminal){
    return current->value;
  }
  return NULL;
}

/*void hello(){
  printf("Hola mundo\n");
}

int main(){
  RadixTree* rdt = createRadixTree();
  insert(rdt, "api/users", hello);
  insert(rdt, "otra/dos", NULL);
  insert(rdt, "api/products/algo", NULL);
  insert(rdt, "api/products/otracosa", NULL);
  insert(rdt, "otra/una", NULL);
  //iterate(rdt, rdt->root);

  ((void (*)())get(rdt, "api/users"))();
}*/



