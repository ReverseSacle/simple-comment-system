#include"./_http/http.h"
#include"./_tcp/tcp.h"
#include<signal.h>

TcpStream server_tcp;

void ExitAction(int sig)
{	
	exit(0);
}

int main()
{
	// 屏蔽所有Linux信号
	for(int i = 0;i < 100;++i){ signal(i,SIG_IGN); }
	signal(2,ExitAction);// 捕获Ctrl c中断 信号
	signal(15,ExitAction);// 捕获Kill命令 信号

	if(false == server_tcp.Construct(80)){ return 0; }
	if(false == server_tcp.ToListen()){ return 0; }
	
	int sock_fd = server_tcp.tcp_sock_fd;
	size_t sock_addr_len = sizeof(struct sockaddr_in);

	while(true)
	{
		int accept_fd = -1;
		struct sockaddr_in sock_addr;

		// log...
		std::cout << "main() => Waiting server connect..." << std::endl;
		accept_fd = accept(sock_fd,(struct sockaddr*)&sock_addr,(socklen_t*)&sock_addr_len);
		// log...
		std::cout << "main() => Server connected" << std::endl;

		HttpServer::HttpAccept(accept_fd);
		close(accept_fd);
	}		
	return 0;
}
