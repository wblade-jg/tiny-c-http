#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>

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
} HttpRequest;

typedef struct httpResponse{
   char* httpVersion;
   int statusCode;
   char* message;

   unsigned char* body;
   size_t bodySize; 
} HttpResponse;

HttpRequest* parseHttp(char* data);

typedef void (*handler)(HttpRequest*, HttpResponse*); 
#endif
