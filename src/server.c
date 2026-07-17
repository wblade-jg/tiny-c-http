#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include "router.h"
#include "server.h"
#include <pthread.h>
#include <unistd.h>

void* handle_request(void* fd_pointer);

ServerBuilder* createServerBuilder(){
  ServerBuilder* builder = malloc(sizeof(ServerBuilder));
  return builder;
}

void port(ServerBuilder* builder, int port){
  builder->port = port;
}

void backlogSize(ServerBuilder* builder, int backlogSize){
  builder->backlogSize = backlogSize;
}

void mapImplementation(ServerBuilder* builder, char* map){
  builder->map = map;
}

Server* createServer(ServerBuilder* builder){
  Server* server = malloc(sizeof(Server)); 
  server->fd = socket(AF_INET, SOCK_STREAM, 0);
  server->port = builder->port;

  if(server->fd < 0){
    perror("Error al crear el socket");
    return NULL;
  }
  
  int opt = 1;
  setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(builder->port);
  address.sin_addr.s_addr = INADDR_ANY;
  memset(&address.sin_zero, 0, sizeof(address.sin_zero));

  if(bind(server->fd, (struct sockaddr *)&address, sizeof(address)) < 0){
    perror("Error en el bind\n");
    return NULL;
  }
  
  listen(server->fd, builder->backlogSize);
  
  createRouter(builder->map);
  
  return server;
}

void dispatch(int socketDescriptor){
  pthread_t idHilo;
  pthread_create(&idHilo, NULL, handle_request, (void *)(long)socketDescriptor);
}

void run(Server* server){
  printf("Servidor escuchando en el puerto: %d\n", server->port);
  while(1){
    struct sockaddr_in clientAddress;
    socklen_t size = sizeof(clientAddress);
    int client_fd = accept(server->fd, (struct sockaddr*)&clientAddress, &size);

    if(client_fd < 0){
      perror("Error al ejecutar el servidor");
      return;
    }
    dispatch(client_fd);
  }
}

HttpRequest* parseHttp(char* data){
  HttpRequest* req = malloc(sizeof(HttpRequest));
  req->method =  strtok(data, " ");
  req->uri = strtok(NULL, " ");
  req->isHandled = false;
  return req;
}

void sendHttpResponse(int socketDescriptor, HttpResponse* response){
  char sendBuffer[1024];
  
  char* contentType = response->contentType ? response->contentType : "text/html";
  int headersLen = snprintf(sendBuffer, sizeof(sendBuffer), 
      "HTTP/1.1 %d %s\r\n"
      "Content-Length: %ld\r\n"
      "Content-Type: %s\r\n"
      "\r\n", 
      response->statusCode, response->message, response->bodySize, contentType);
  
  send(socketDescriptor, sendBuffer, headersLen, 0);

  if (response->body != NULL && response->bodySize > 0) {
      send(socketDescriptor, response->body, response->bodySize, 0);
  }
}

  
void* handle_request(void* fd_pointer){
  const int bufferSize = 8192;
  char buffer[bufferSize];

  int fd = (int)(long)fd_pointer;

  while(1){
    memset(buffer, 0, bufferSize);

    int bytesRecibidos = recv(fd, buffer, bufferSize, 0);
    
    if(bytesRecibidos > 0){
      HttpRequest* req = parseHttp(buffer);
      HttpResponse* res = malloc(sizeof(HttpResponse));
      route(req, res);
      sendHttpResponse(fd, res);
      free(req);
      free(res);
    }else if(bytesRecibidos == 0){
      printf("Cliente desconectado\n");
      break;
    }else{
      perror("Error al leer buffer de recepcion");
      break;
    }
  }
  close(fd);
  return NULL;  
}
