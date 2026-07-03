#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdbool.h>

typedef struct httpHeader{
  char* name;
  char* value;
} HttpHeader;

typedef struct httpRequest {
  char* method;
  char* uri;
  char* version;
  
  HttpHeader* headers[50];
  char* body;
  bool isHandled;
} HttpRequest;

typedef struct httpResponse{
   char* httpVersion;
   int statusCode;
   char* message;

   unsigned char* body;
   size_t bodySize;
   char* contentType;
} HttpResponse;

HttpRequest* parseHttp(char* data);

typedef void (*handler)(HttpRequest*, HttpResponse*); 
#endif
