#pragma once
#include"_response.h"

struct ReqContent
{
    char req_method[256];
    char req_url[1024];
    char req_body[4096];
    size_t url_len;
};

void HttpServer_Accept(int sock_fd);
