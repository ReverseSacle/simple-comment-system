#pragma once
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/epoll.h>
#include"_clibs.h"
#include"database.h"

#define MAXCLIENTS 100

enum FileType
{
	_HTML = 1,
	_CSS = 2,
	_JS = 3,
	_PNG = 4
};

enum RequestMethod
{
	_GET = 1,
	_POST = 2,
	_PUT = 3
};

enum RequestURL
{
    _DATABASE = 1,
    _API = 2
};

int ServerSocketConstruct(const u16 port);
void HttpAccept(i32 sockfd);
void HttpResponse(i32 sockfd,const char* url);
