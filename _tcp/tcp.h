#pragma once
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include"../mylibs.h"

// 允许所有IP连接
#define _IPUSER INADDR_ANY

class TcpStream
{
private:
	struct sockaddr_in sock_addr;// socket通信地址结构体
	size_t sock_addr_size = sizeof(sock_addr);// 通信地址结构体字节大小
public:
	int tcp_sock_fd = -1;// 申请的socket资源
	TcpStream();
	~TcpStream();
	bool Construct(const size_t port);
	bool ToListen();// 将socket设置为监听状态
	static bool StreamParse_Line(int sock_fd,std::string& buffer);// 读取stream流中的一行
};
