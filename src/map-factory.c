#include "treemap.h"
#include "radixtreemap.h"
#include <string.h>
#include <stdio.h>
#include "mapfactory.h"
#include <stdlib.h>

void* mapFactory(char* type){
  Map* map = malloc(sizeof(Map));

  if(strcmp(type, "Treemap") == 0){
    printf("Usando Treemap\n");
    map->impl = createTreemap();
    map->insert = treemapInsert;
    map->get = treemapGet;
  }else if(strcmp(type, "RadixTree") == 0){
    printf("Usando RadixTree\n");
    map->impl = createRadixTree();
    map->insert = radixInsert; 
    map->get = radixGet;
  }
  return map;
}
