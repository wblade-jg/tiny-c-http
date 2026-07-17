#include "router.h"
#include "mapfactory.h"
#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArrayList* middlewares;
Map* map;

typedef struct middleware{
  handler func;
  char* route;
} Middleware;

void createRouter(char* mapType){
  map = mapFactory(mapType);
  middlewares = createArrayList(10); 
  map->insert(map->impl, "GET", mapFactory(mapType));
}

void addMiddleware(char* route, handler func){
  Middleware* middleware = malloc(sizeof(Middleware));
  middleware->route = strdup(route);
  middleware->func = func;
  push(middlewares, middleware);
  printf("Middleware agregado correctame\n");
}

void addGet(char* ruta, handler func){
  Map* mapGet = map->get(map->impl, "GET");
  mapGet->insert(mapGet->impl, ruta, func);
}

bool starts_with(char* substring, char* string){
  for(size_t i = 0; i < strlen(substring); i++){
    if(substring[i] != string[i]){
      return false;
    }
  }
  return true;
}

void route(HttpRequest* req, HttpResponse* res){
  int index = 0;

  while(req->isHandled==false){
    Middleware* middleware = (Middleware *)getAt(middlewares, index);
    if(middleware == NULL){
      Map* m = map->get(map->impl, req->method);
      if(m == NULL){
        res->statusCode = 405;
        res->message = "Method Not Allowed";
        res->bodySize = 0;
        req->isHandled = true;
        break;
      }
      handler func = (handler)m->get(m->impl, req->uri);
      if(func != NULL){
        func(req, res);
        req->isHandled = true;
        continue;
      }else{
        res->statusCode = 404;
        res->message = "Not Found";
        res->bodySize = 0;
        req->isHandled = true;
        break;
      }
    }
    if(starts_with(middleware->route, req->uri)){
      middleware->func(req, res);
    }

    index++;
  }

  
}




