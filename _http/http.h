#pragma once
#include"./_response.h"

// HTTP 请求链接的切分内容
struct ReqContent
{
	std::string req_method;
	std::string req_url;
	std::string req_body;
};

class HttpServer{
private:
	static std::string UrlFormat(std::string& url_buf);
	static bool ReqContentReceive(int sock_fd,ReqContent* req_content);
	static void BodyTackle(const std::string& body_buf,Record* record);
	static void GetMethod(int sock_fd,const std::string& url_buf);
	static void PostMethod(int sock_fd,const std::string& body_buf);
public:
	static void HttpAccept(int sock_fd);
};

