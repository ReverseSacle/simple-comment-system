#pragma once
#include<fstream>
#include<sys/stat.h>
#include"../mylibs.h"
#include"../_database/db.h"
#include"../_tcp/tcp.h"

#define _ROOTDIR "docs"
#define _HTTPVERSION "1.0"

// HTTP请求方法
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

// HTTP请求文件格式
enum FileType
{
    _HTML = 1,
    _CSS = 2,
    _JS = 3,
    _PNG = 4
};

// 请求路径功能所属
enum RequestPathType
{
    _DATABASE = 1,
    _API = 2
};

class TcpResponse
{
private:
    static void Response404(int sock_fd);
    static void Response200_NF(int sock_fd);// 200状态的普通响应
    static void Response200_DB(int sock_fd);// 200状态的数据库响应
    // 200状态的文件请求响应
    static void Response200(int sock_fd,const std::string& path,const off_t f_size);
public:
    static void Response400(int sock_fd);
    static int GetMethodType(const std::string& method_buf);// 获取HTTP请求方法
    static int GetFileType(const std::string& filename_buf);// 获取HTPP请求文件类型
    static int GetPathType(const std::string& path_buf);// 获取HTTP请求路径类型
    static void ResponseSelector(int sock_fd,const std::string& url_buf);// 状态响应选择器
};
