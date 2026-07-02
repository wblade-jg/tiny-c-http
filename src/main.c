#include <string.h>
#include "server.h"
#include "router.h"

void hello(HttpRequest* req, HttpResponse* res){
  printf("%s - Hello World\n", req->method);
  res->statusCode = 200;
  res->body = (unsigned char*)"Hello desde mi servidor web :)\n";
  res->bodySize = strlen((char*)res->body);
  res->message = "OK";
} 

int main(){
  ServerBuilder* builder = createServerBuilder();
  port(builder, 8080);
  backlogSize(builder, 10);
  mapImplementation(builder, "Treemap");

  Server* server = createServer(builder);
  addGet("/", hello);
  run(server);
}

