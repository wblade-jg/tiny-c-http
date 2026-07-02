#include "router.h"
#include "mapfactory.h"

Map* map;

void createRouter(char* mapType){
  map = mapFactory(mapType);
  map->insert(map->impl, "GET", mapFactory(mapType));
}

void addGet(char* ruta, handler func){
  Map* mapGet = map->get(map->impl, "GET");
  mapGet->insert(mapGet->impl, ruta, func);
}

void route(HttpRequest* req, HttpResponse* res){
  Map* m = map->get(map->impl, req->method);
  handler func = (handler)m->get(m->impl, req->uri);
  if(func != NULL){
    func(req, res);
  }else{
    printf("ruta no existe\n");
  }
}



