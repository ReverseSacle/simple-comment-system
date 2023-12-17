#include<stdlib.h>
#include<signal.h>
#include"./_http/http.h"
#include"./_tcp/tcp.h"

void ExitAction(int sig)
{
	exit(0);
}

int main()
{
	int i = 0;
	while(i < 100){ signal(i++,SIG_IGN); }
	signal(2,ExitAction);
	signal(15,ExitAction);

	int sock_fd = -1;
	if(-1 == (sock_fd = TcpStream_Construct(80))){ return 0; }
	if(-1 == TcpStream_ToListen(sock_fd)){ return 0; }

	int accept_fd = -1;
	struct sockaddr_in sock_addr;
	size_t sock_addr_size = sizeof(struct sockaddr_in);
	while(true)
	{
		_LogFileWrite(_LOGPATH,1,"main() => Waiting server connect...\n");
		_CallDebug(1,"main() => Waiting server connect...\n");

		accept_fd = accept(sock_fd,(struct sockaddr*)&sock_addr,(socklen_t*)&sock_addr_size);

		_LogFileWrite(_LOGPATH,1,"main() => Client connected\n");
		_CallDebug(1,"main() => Client connected\n");

		HttpServer_Accept(accept_fd);
		close(accept_fd);
	}
	return 0;
}
