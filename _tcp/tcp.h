#pragma once
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"../mylibs.h"

// 允许所有IP连接
#define _IPUSER AF_INET

class TcpStream{
private:
	struct sockaddr_in sock_addr;// socket通信地址结构体
	size_t sock_addr_size = sizeof(sock_addr);// 通信地址结构体字节大小
public:
	int tcp_sock_fd = -1;// 申请的socket资源
	TcpStream();// 配置socket通信地址结构体(协议族，通信目标地址)
	~TcpStream();// socket断开时自动退出
	/**
		* Construct()，用于
		* socket通道建立，包含socket资源申请(socket())，
		* 通信地址结构体的通信端口绑定(sin_port)，
		* 通道地址绑定到socket(bind())。
	**/
	bool Construct(const size_t port);
	bool ToListen();// 将socket设置为监听状态
	static bool StreamParse_Line(int sock_fd,std::string& buffer);// 读取stream流中的一行
};
