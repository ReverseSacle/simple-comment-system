#include"tcp.h"

/* Tcp流创建及通信地址绑定 */
int TcpStream_Construct(const usize_t port)
{
	if(port > 65535){ return -1; }

	int sock_fd = -1;
	if(-1 == (sock_fd = socket(AF_INET,SOCK_STREAM,0)))
	{
		_LogFileWrite(_LOGPATH,1,"TcpStream_Construct() => socket() fail\n");
		_CallDebug(1,"TcpStream_Construct() => socket() fail\n");
		return -1;
	}

	/* 设置通信地址 */
	struct sockaddr_in sock_addr;
	size_t sock_addr_size = sizeof(sock_addr);

	memset(&sock_addr,0,sock_addr_size);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(port);
	
	/* 初始化通信端口 */
	int opt = 1;
	setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	

	/* 绑定通信地址 */
	if(-1 == bind(sock_fd,(struct sockaddr*)&sock_addr,sock_addr_size))
	{
		_LogFileWrite(_LOGPATH,1,"TcpStream_Construct() => bind() fail\n");
		_CallDebug(1,"TcpStream_Construct() => bind() fail\n");

		close(sock_fd);
		return -1;
	}
	return sock_fd;
}

/* 设置监听模式 */
bool TcpStream_ToListen(int sock_fd)
{
	if(-1 == listen(sock_fd,5))
	{
		_LogFileWrite(_LOGPATH,1,"TcpStream_Construct() => listen() fail\n");
		_CallDebug(1,"TcpStream_Construct() => listen() fail\n");

		close(sock_fd);
		return false;
	}
	return true;
}

/* 从Tcp流中读取一行,不适用body部分的读取 */
size_t TcpStream_StreamParseLine(int sock_fd,char* buf,const size_t buf_size)
{
	if(-1 == sock_fd){ return 0; }

	char ch = '\0';
	size_t index = 0;

	while(index < buf_size) 
	{
		ssize_t len = recv(sock_fd,&ch,1,0);
		if(1 == len)
		{
			if('\r' == ch){ continue; }
			if('\n' == ch){ break; }
			buf[index++] = ch;
		}
		else 
		{	
			if(-1 == len)
			{ 
				_LogFileWrite(_LOGPATH,1,"TcpStream_StreamParseLine() => recv() fail\n");
				_CallDebug(1,"TcpStream_StreamParseLine() => recv() fail\n");
				index = 0;
			}
			else
			{ 
				_LogFileWrite(_LOGPATH,1,"TcpStream_StreamParseLine() => recv() end\n");
				_CallDebug(1,"TcpStream_StreamParseLine() => recv() end\n");
			}
			break;
		}
	}		
	buf[index] = '\0';
	return index;
}
