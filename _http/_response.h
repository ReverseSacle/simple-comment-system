#pragma once
#include<sys/stat.h>
#include<errno.h>
#include"../mylibs.h"
#include"../_tcp/tcp.h"
#include"../_database/db.h"

#define _ROOTDIR "docs"
#define _HTTPVERSION "1.0"

enum RequestMethod
{
    _GET = 1,
    _POST = 2,
    _PUT = 3,
    _DELETE = 4,
    _HEAD = 5,
    _OPTIONS = 6,
    _PATCH = 7,
    _CONNECT = 8,
    _TRACE = 9
};

enum FileType
{
    _HTML = 1,
    _CSS = 2,
    _JS = 3,
    _PNG = 4
};

enum RequestPathType
{
    _DATABASE = 1,
    _API = 2
};

int GetMethodType(const char* method_buf);
int GetFileType(const char* filename_buf);
int GetPathType(const char* path_buf);
void Response400(int sock_fd);
void ResponseSelector(int sock_fd,const char* url_buf);
