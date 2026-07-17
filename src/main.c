#include <string.h>
#include "server.h"
#include "router.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define PATH_TO_SEARCH "./public"

static char* mime_type(char* ext){
  if(ext == NULL) return "application/octet-stream";
  if(strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
  if(strcmp(ext, ".css") == 0)  return "text/css";
  if(strcmp(ext, ".js") == 0)   return "application/javascript";
  if(strcmp(ext, ".json") == 0) return "application/json";
  if(strcmp(ext, ".png") == 0)  return "image/png";
  if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
  if(strcmp(ext, ".gif") == 0)  return "image/gif";
  if(strcmp(ext, ".svg") == 0)  return "image/svg+xml";
  if(strcmp(ext, ".ico") == 0)  return "image/x-icon";
  if(strcmp(ext, ".txt") == 0)  return "text/plain";
  if(strcmp(ext, ".pdf") == 0)  return "application/pdf";
  return "application/octet-stream";
}

static char* get_ext(char* uri){
  char* dot = strrchr(uri, '.');
  if(!dot || strchr(dot, '/')) return NULL;
  return dot;
}

void static_router(HttpRequest* req, HttpResponse* res){
  char* archivo_buscar = (req->uri[0] == '/') ? req->uri + 1 : req->uri;
  char ruta_completa[512];
  snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", PATH_TO_SEARCH, archivo_buscar);

  FILE* file = fopen(ruta_completa, "rb");
  if(file == NULL){
    res->statusCode = 404;
    res->message = "Not Found";
    res->bodySize = 0;
    req->isHandled = true;
    return;
  }

  char* ext = get_ext(req->uri);
  res->contentType = mime_type(ext);

  fseek(file, 0, SEEK_END);
  long tamano = ftell(file);
  fseek(file, 0, SEEK_SET);

  res->body = (unsigned char*)malloc(tamano + 1);
  if(res->body == NULL){
    res->statusCode = 500;
    res->message = "Internal Server Error";
    res->bodySize = 0;
    fclose(file);
    req->isHandled = true;
    return;
  }

  fread(res->body, 1, tamano, file);
  res->body[tamano] = '\0';
  res->bodySize = tamano;
  res->statusCode = 200;
  res->message = "OK";
  fclose(file);
  req->isHandled = true;
}

void hello(HttpRequest* req, HttpResponse* res){
  printf("%s - Hello World\n", req->method);
  res->statusCode = 200;
  res->contentType = "text/html";
  res->body = (unsigned char*)"Hello desde mi servidor web :)\n";
  res->bodySize = strlen((char*)res->body);
  res->message = "OK";
  req->isHandled = true;
} 

int get_port(void) {
    const char* port_env = getenv("PORT");
    if (port_env != NULL) {
        return atoi(port_env);
    }
    return 8080; 
}

int main(){
  ServerBuilder* builder = createServerBuilder();
  port(builder, get_port());
  backlogSize(builder, 10);
  mapImplementation(builder, "Treemap");

  Server* server = createServer(builder);
  
  if (server == NULL) {
    fprintf(stderr, "No se pudo iniciar el servidor\n");
    exit(1);
  } 
  
  addMiddleware("/", static_router);
  addGet("/", hello);
  run(server);
}

