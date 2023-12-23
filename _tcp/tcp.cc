#include"tcp.h"

TcpStream::TcpStream()
{
	memset(&sock_addr,0,sock_addr_size);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(_IPUSER);
}

TcpStream::~TcpStream()
{
	if(-1 == tcp_sock_fd){ return; }
	close(tcp_sock_fd);
}

bool TcpStream::Construct(const size_t port)
{
	if(-1 != tcp_sock_fd){ return true; }
	if(port > 65535){ return false; }

	tcp_sock_fd = -1;
	if(-1 == (tcp_sock_fd = socket(AF_INET,SOCK_STREAM,0)))
	{
		// log...
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
		MyLibs::CallDebug("TcpStream::Construct() => bind fail");
		close(tcp_sock_fd);
		return false;
	}
	// log...
	MyLibs::CallDebug("TcpStream::Construct() => Succeed");
	return true;
}

bool TcpStream::ToListen()
{
	if(-1 == tcp_sock_fd){ return false; }

	if(-1 == listen(tcp_sock_fd,5))
	{
		// log...
		MyLibs::CallDebug("TcpStream::ToListen() => listen() fail");
		close(tcp_sock_fd);
		return false;
	}
	// log...
	MyLibs::CallDebug("TcpStream::ToListen() => Succeed");
	return true;
}

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
				MyLibs::CallDebug("TcpStream::StreamParse_Line() => recv() fail");
				buffer.clear();
				return false;
			}
			else
			{
				// log...
				MyLibs::CallDebug("TcpStream::StreamParse_Line() => end of recv()");
				break;
			}
		}
	}
	return true;
}

