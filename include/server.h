#ifndef SERVER_H
#define SERVER_H

typedef struct serverBuilder{
  int port;
  int backlogSize;
  char* map;
} ServerBuilder;

typedef struct server{
  int port;
  int fd;
}Server;

ServerBuilder* createServerBuilder();
void port(ServerBuilder* builder, int port);
void backlogSize(ServerBuilder* builder, int backlogSize);
void mapImplementation(ServerBuilder* builder, char* map);
Server* createServer(ServerBuilder* builder);
void run(Server* server);

#endif
