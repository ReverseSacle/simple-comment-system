#pragma once
#include<sys/stat.h>
#include"../mylibs.h"
#include"../_database/db.h"
#include"../_tcp/tcp.h"

#define _HTTPVERSION "1.0"

// HTTP请求方法
enum class RequestMethodType
{
    _UNKNOW,
    _GET,
    _POST,
    _PUT,
    _DELETE,
    _HEAD,
    _OPTIONS,
    _PATCH,
    _CONNECT,
    _TRACE
};

// HTTP请求文件格式
enum class RequestFileType
{
    _UNKNOW,
    _HTML = 1,
    _CSS = 2,
    _JS = 3,
    _PNG = 4
};

// 请求路径功能所属
enum class RequestPathType
{
    _NORMAL,
    _DATABASE,
    _DATABASE_COUNT,
    _DATABASE_REPLY,
    _API
};

class TcpResponse
{
private:
    static void Response200_NF(int sock_fd);
    static void Response200_DB(int sock_fd);
    static void Response200_DBR(int sock_fd);
    static void Response200_DBC(int sock_fd);
    static void Response200(int sock_fd,const std::string& path,const off_t f_size);
public:
    static void Response400(int sock_fd);
    static void Response404(int sock_fd);
    static RequestMethodType GetMethodType(const std::string& method_buf);
    static RequestFileType GetFileType(const std::string& filename_buf);
    static RequestPathType GetPathType(const std::string& path_buf);
    static void ResponseSelector(int sock_fd,const std::string& url_buf);
};
