#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef TreeNode Node;

Node* createNode(char* key, void* value){
  Node* node = malloc(sizeof(Node));
  node->color = RED;
  node->key = key ? strdup(key) : NULL;
  node->value = value;
  return node;
}

Treemap* createTreemap(){
  Treemap* tree = malloc(sizeof(Treemap));
  tree->NIL = createNode(NULL, 0);
  tree->NIL->color = BLACK;
  tree->root = tree->NIL;
  return tree;
}

void rotateToLeft(Treemap* tree, Node* x){
  Node* y = x->right;
  x->right = y->left;

  if(y->left!=tree->NIL) y->left->parent = x;
  y->parent = x->parent;

  if(x->parent == NULL) tree->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void rotateToRight(Treemap* tree, Node* x){
  Node* y = x->left;
  x->left = y->right;

  if(y->right!=tree->NIL) y->right->parent = x;
  y->parent = x->parent;

  if(x->parent == NULL) tree->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;
  
  y->right = x;
  x->parent = y;
}

void fixUpInsert(Treemap* tree, Node* node){
  while(node!=tree->root && node->parent->color == RED){
    if(node->parent == node->parent->parent->left){
      Node* uncle = node->parent->parent->right;
      
      if(uncle->color == RED){
        node->parent->color = BLACK;
        uncle->color = BLACK;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      }else{
        if(node == node->parent->right){
          node = node->parent;
          rotateToLeft(tree, node);
        }
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        rotateToRight(tree, node->parent->parent);
      }
    }else{
      Node* uncle = node->parent->parent->left;

      if(uncle->color == RED){
        node->parent->color = BLACK;
        uncle->color = BLACK;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      }else{
        if(node == node->parent->left){
          node = node->parent;
          rotateToRight(tree, node);
        }
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        rotateToLeft(tree, node->parent->parent);
      }
    }
  }
  tree->root->color = BLACK;
}

void treemapInsert(void* m, char* key, void* value){
  Treemap* tree = (Treemap *)m;
  Node* node = createNode(key, value);
  node->left = node->right = tree->NIL;
  
  if(tree->root == tree->NIL){
    tree->root = node;
    node->color = BLACK;
    return;
  }

  Node* parent = NULL;
  Node* current = tree->root;
  
  while(current!=tree->NIL){
    parent = current;
    
    if(strcmp(node->key, current->key) < 0){ 
      current = current->left;
    }else if(strcmp(node->key, current->key) == 0){ 
      current->value = value;
      free(node);
      return;
    }else{ 
      current = current->right;
    }
  }

  node->parent = parent;
  
  if(strcmp(node->key, node->parent->key) < 0) parent->left = node;
  else parent->right = node;
  
  if(node->parent->parent == NULL) return;
  fixUpInsert(tree, node);
}

void* treemapGet(void* m, char* key){
  Treemap* tree = (Treemap* )m;
  Node* current = tree->root;

  while(current!=tree->NIL){
    int result = strcmp(key, current->key);
    if(result == 0){
      return current->value;
    }else if(result < 0){
      current = current->left;
    }else{
      current = current->right;
    }
  }
  
  return NULL;
}
