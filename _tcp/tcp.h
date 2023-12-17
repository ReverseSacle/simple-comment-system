#pragma once
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"../mylibs.h"

#define _IPUSER AF_INET

int TcpStream_Construct(const usize_t port);
bool TcpStream_ToListen(int sock_fd);
size_t TcpStream_StreamParseLine(int sock_fd,char* buf,const size_t buf_size);
