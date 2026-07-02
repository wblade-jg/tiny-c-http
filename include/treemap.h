#ifndef TREEMAP_H
#define TREEMAP_H

enum Color{
  RED,
  BLACK
};

typedef struct treenode{
  char* key;
  void* value;
  enum Color color;
  struct treenode* left;
  struct treenode* right;
  struct treenode* parent;
}TreeNode;

typedef struct tree{
  TreeNode* root;
  TreeNode* NIL;
} Treemap;

Treemap* createTreemap();
void treemapInsert(void* m, char* key, void* value);
void* treemapGet(void* m, char* key);

#endif
