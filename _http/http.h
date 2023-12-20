#pragma once
#include"./_response.h"

struct ReqContent
{
	std::string req_method;
	std::string req_url;
	std::string req_body;
};

class HttpServer{
private:
	static std::string UrlFormat(std::string& url_buf);// 将url的格式标准化
	static bool ReqContentReceive(int sock_fd,ReqContent* req_content);// HTTP请求内容解析
	static void BodyTackle(const std::string& body_buf,Record* record);// HTTP请求Body部分解析
	static void GetMethod(int sock_fd,const std::string& url_buf);// 处理HTTP的GET方法
	static void PostMethod(int sock_fd,const std::string& body_buf);// 处理HTTP的POST方法
public:
	static void HttpAccept(int sock_fd);// 接收HTTP请求报文，并处理相关操作
};

