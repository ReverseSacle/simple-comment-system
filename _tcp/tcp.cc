#include"tcp.h"

// 构造函数，配置socket通信地址结构体(协议族，通信目标地址)
TcpStream::TcpStream()
{
	memset(&sock_addr,0,sock_addr_size);
	sock_addr.sin_family = AF_INET;// 接入的地址类型
	sock_addr.sin_addr.s_addr = htonl(_IPUSER);// 允许接入的IP用户
}

// 析构函数，socket断开时自动退出
TcpStream::~TcpStream()
{
	if(-1 == tcp_sock_fd){ return; }
	close(tcp_sock_fd);
}


/**
 * 服务端Tcp连接建立函数
 * 用于socket通道建立，包含socket资源申请，
 * 通信地址结构体的通信端口绑定，通道地址绑定到。
**/
bool TcpStream::Construct(const size_t port)
{
	if(-1 != tcp_sock_fd){ return true; }
	if(port > 65535){ return false; }

	tcp_sock_fd = -1;
	if(-1 == (tcp_sock_fd = socket(AF_INET,SOCK_STREAM,0)))
	{
		// log...
		MyLibs::CallLogError("TcpStream::Construct() => tcp_sock_fd fail");
		MyLibs::CallDebug("TcpStream::Construct() => tcp_sock_fd fail");
		return false;
	}
	sock_addr.sin_port = htons(port);

	// 立即初始化端口的占用
	int opt = 1;
	setsockopt(tcp_sock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	

	if(-1 == bind(tcp_sock_fd,(struct sockaddr*)(&sock_addr),sock_addr_size))
	{
		// log...
		MyLibs::CallLogError("TcpStream::Construct() => bind fail");
		MyLibs::CallDebug("TcpStream::Construct() => bind fail");
		close(tcp_sock_fd);
		return false;
	}
	// log...
	MyLibs::CallLogInfo("TcpStream::Construct() => Succeed");
	MyLibs::CallDebug("TcpStream::Construct() => Succeed");
	return true;
}

// 设置监听模式
bool TcpStream::ToListen()
{
	if(-1 == tcp_sock_fd){ return false; }

	if(-1 == listen(tcp_sock_fd,5))
	{
		// log...
		MyLibs::CallLogError("TcpStream::ToListen() => listen() fail");
		MyLibs::CallDebug("TcpStream::ToListen() => listen() fail");
		close(tcp_sock_fd);
		return false;
	}
	// log...
	MyLibs::CallLogInfo("TcpStream::ToListen() => Succeed");
	MyLibs::CallDebug("TcpStream::ToListen() => Succeed");
	return true;
}


/**
 * 读取TCP字节流中的一行数据
 * 一个字节一个字节的读取，通过判断`\n\r`字符串来确定是否终止
**/
bool TcpStream::StreamParse_Line(int sock_fd,std::string& buffer)
{
	if(-1 == sock_fd){ return false; }	

	char ch = '\0';
	while(true)
	{
		int len = recv(sock_fd,&ch,1,0);
		if(1 == len)
		{
			if('\r' == ch){ continue; }
			if('\n' == ch){ break; }
			buffer += ch;
		}
		else 
		{
			if(-1 == len)
			{
				// log...
				MyLibs::CallLogError("TcpStream::StreamParse_Line() => recv() fail");
				MyLibs::CallDebug("TcpStream::StreamParse_Line() => recv() fail");
				buffer.clear();
				return false;
			}
			else
			{
				// log...
				MyLibs::CallLogInfo("TcpStream::StreamParse_Line() => end of recv()");
				MyLibs::CallDebug("TcpStream::StreamParse_Line() => end of recv()");
				break;
			}
		}
	}
	return true;
}

