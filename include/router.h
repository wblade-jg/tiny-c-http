#ifndef ROUTER_H
#define ROUTER_H

#include "http.h"

void createRouter(char* mapType);
void addGet(char* ruta, handler func);
void route(HttpRequest* req, HttpResponse* res);

#endif
