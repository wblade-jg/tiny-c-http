#ifndef MAPFACTORY_H
#define MAPFACTORY_H

typedef struct map{
  void* impl;
  void (*insert)(void* m, char* key, void* value);
  void* (*get)(void* m, char* key);
}Map;

void* mapFactory(char* type);
#endif
