#include"http.h"

i32 ServerSocketConstruct(const u16 port)
{
	if(port > 65535) return - 1;

	i32 server_fd = -1;
	if(-1 == (server_fd = socket(AF_INET,SOCK_STREAM,0)))
	{
		_LogFileWrite(_LOGPATH,1,"SocketConstruct: socket() Error\n");
		_CallDebug(1,"SocketConstruct: socket() Error\n");
		return -1;
	}

	struct sockaddr_in server_addr;
	u32 server_addrSize = sizeof(server_addr);

	memset(&server_addr,0,server_addrSize);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	/* Initial port */
	i32 opt = 1;
	u32 optSize = sizeof(opt);
	setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,optSize);	

	if(-1 == bind(server_fd,(struct sockaddr*)&server_addr,server_addrSize))
	{
		_LogFileWrite(_LOGPATH,1,"SocketConstruct: bind() Error\n");
		_CallDebug(1,"SocketConstruct: bind() Error\n");
		close(server_fd);
		return -1;
	}

	if(-1 == listen(server_fd,5))
	{
		_LogFileWrite(_LOGPATH,1,"SocketConstruct: listen() Error\n");
		_CallDebug(1,"SocketConstruct: listen() Error\n");
		close(server_fd);
		return -1;
	}
	return server_fd;
}

static i32 GetLine(i32 sockfd,char* buf,u32 size)
{
	char ch = '\0';
	i32 index = 0;
	while(index < size - 1)
	{
		i32 len = recv(sockfd,&ch,1,0);
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
				_LogFileWrite(_LOGPATH,1,"GetLine: recv() Error\n");
				_CallDebug(1,"GetLine: recv() Error\n");
			}
			else
			{ 
				_LogFileWrite(_LOGPATH,1,"GetLine: Client Closed.\n");
				_CallDebug(1,"GetLine: Client Closed.\n");
			}
			index = 0;
			break;
		}
	}		
	buf[index] = '\0';
	return index;
}

static i32 GetMethodType(const char* method)
{
	i32 select[3] = {0};
	while('\0' != *method)
	{
		char c = toupper(*method);
		if('P' == c || 'O' == c || 'S' == c || 'T' == c){ ++select[0]; }
		if('G' == c || 'E' == c || 'T' == c){ ++select[1]; }
		if('P' == c || 'U' == c || 'T' == c){ ++select[2]; }
		++method;
	}
	if(4 == select[0])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetMethodType: POST\n");
		_CallDebug(1,"GetMethodType: POST\n"); 
		return _POST; 
	}
	if(3 == select[1])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetMethodType: GET\n");
		_CallDebug(1,"GetMethodType: GET\n"); 
		return _GET; 
	}
	if(3 == select[2])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetMethodType: PUT\n");
		_CallDebug(1,"GetMethodType: PUT\n"); 
		return _PUT; 
	}
	_LogFileWrite(_LOGPATH,1,"GetMethodType: UNKNOW\n");
	_CallDebug(1,"GetMethodType: UNKNOW\n"); 
	return -1;
}

static i32 GetFileType(const char* ostr)
{
	char path[64] = {'\0'};
	char* ch = NULL,* cache = NULL;
	i32 select[4] = {0};
	
	strcpy(path,ostr);
	for(ch = strtok(path,".");NULL != ch;ch = strtok(NULL,"."))
		cache = ch;
	
	while('\0' != *cache)
	{
		char c = toupper(*cache);
		if('H' == c || 'T' == c || 'M'== c || 'L' == c){ ++select[0]; }
		if('C' == c || 'S' == c){ ++select[1]; }
		if('J' == c || 'S' == c){ ++select[2]; }
		if('P' == c || 'N' == c || 'G' == c){ ++select[3]; }
		++cache;
	}

	if(4 == select[0])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetFileType: HTML\n");
		_CallDebug(1,"GetFileType: HTML\n"); 
		return _HTML; 
	}
	if(3 == select[1])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetFileType: CSS\n");
		_CallDebug(1,"GetFileType: CSS\n"); 
		return _CSS; 
	}
	if(2 == select[2])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetFileType: JS\n");
		_CallDebug(1,"GetFileType: JS\n"); 
		return _JS; 
	}
	if(3 == select[3])
	{ 
		_LogFileWrite(_LOGPATH,1,"GetFileType: PNG\n");
		_CallDebug(1,"GetFileType: PNG\n"); 
		return _PNG; 
	}
	_LogFileWrite(_LOGPATH,1,"GetFileType: UNKNOW\n");
	_CallDebug(1,"GetFileType: UNKNOW\n"); 
	return -1;
}

static i32 GetRequestPath(const char* strs)
{
	char* pd = "DATABASE";
	char* pa = "API";

	while('\0' != *strs)
	{
		char c = toupper(*strs);
		if(*pd == c){ ++pd; }
		if(*pa == c){ ++pa; }
		++strs;
	}
	if('\0' == *pd)
	{
		_LogFileWrite(_LOGPATH,1,"GetRequestPath: DATABASE\n");
		_CallDebug(1,"GetRequestPath: DATABASE\n");
		return _DATABASE;
	}
	if('\0' == *pa)
	{
		_LogFileWrite(_LOGPATH,1,"GetRequestPath: API\n");
		_CallDebug(1,"GetRequestPath: API\n");
		return _API;
	}
	_LogFileWrite(_LOGPATH,1,"GetRequestPath: UNKNOW\n");
	_CallDebug(1,"GetRequestPath: UNKNOW\n");
	return -1;	 
} 

static void Response404(i32 sockfd)
{
	char header[128] = {'\0'};
	strcpy(header,"HTTP/1.0 404 NOT FOUND\r\n");
	if(send(sockfd,header,strlen(header),0) <= 0)
	{
		_LogFileWrite(_LOGPATH,3,"Response404: send() Error.reason: ",strerror(errno),"\n");
		_CallDebug(3,"Response404: send() Error.reason: ",strerror(errno),"\n");
	}
}

static void Response400(i32 sockfd)
{
	char header[128] = {'\0'};
	strcpy(header,"HTTP/1.0 400 BAD REQUEST\r\n");
	if(send(sockfd,header,strlen(header),0) <= 0)
	{
		_LogFileWrite(_LOGPATH,3,"Response400: send() Error.reason: ",strerror(errno),"\n");
		_CallDebug(3,"Response400: send() Error.reason: ",strerror(errno),"\n");
	}
}

static void Response200_NF(i32 sockfd)
{
	char buf[1024] = {'\0'};

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	strcat(buf,"Connection: Close\r\n\r\n");

	_CallDebug(2,"Response Header:\n",buf);
	if(send(sockfd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(_LOGPATH,3,"Response200_NF: send() Error.reason: ",strerror(errno),"\n");
		_CallDebug(3,"Response200_NF: send() Error.reason: ",strerror(errno),"\n");
	}
}

static void Response200_DB(i32 sockfd)
{
	char buf[2048] = {'\0'};
	char body[2048] = {'\0'};
	char temp[256] = {'\0'};
	i32 bodyLen = 0;

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	strcat(buf,"Content-Type: text/plain\r\n");
	strcat(buf,"Connection: Close\r\n");

	pthread_mutex_lock(&mutex2);
	MYSQL* mysql = DatabaseConnect();
	MYSQL_RES* res = GetTableElementStart(mysql,"CommentBlock");
	if(NULL != res)
	{
		MYSQL_ROW row;
		while((row = mysql_fetch_row(res)))
		{
			sprintf(temp,"%s:%s:%s:%s,",row[0],row[1],row[2],row[3]);
			strcat(body,temp);
		}
		GetTableElementEnd(res);
	}
	DatabaseConnectClose(mysql);
	pthread_mutex_unlock(&mutex2);
	
	bodyLen = strlen(body);
	sprintf(temp,"Content-Length: %d\r\n\r\n",bodyLen);
	strcat(buf,temp);

	_LogFileWrite(_LOGPATH,2,"Response Header:\n",buf);
	_CallDebug(2,"Response Header:\n",buf);
	if(send(sockfd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(_LOGPATH,3,"Response200_DB: send() Error.reason: ",strerror(errno),"\n");
		_CallDebug(3,"Response200_DB: send() Error.reason: ",strerror(errno),"\n");
	}
	else if(0 != bodyLen)
	{
		if(send(sockfd,body,bodyLen,0) <= 0)
		{
			_LogFileWrite(_LOGPATH,3,"Response200_DB: send() Error.reason: ",strerror(errno),"\n");
			_CallDebug(3,"Response200_DB: send() Error.reason: ",strerror(errno),"\n");
		}
	}
}

static void Response200(i32 sockfd,const char* path)
{
	struct stat st;
	
	if(-1 == stat(path,&st)){ Response404(sockfd); return; }
	char buf[1024] = {'\0'};
	char temp[32] = {'\0'};

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	switch(GetFileType(path))
	{
		case _HTML:{ strcat(buf,"Content-Type: text/html\r\n"); break; }
		case _CSS:{ strcat(buf,"Content-Type: text/css\r\n"); break; }
		case _JS:{ strcat(buf,"Content-Type: text/javascript\r\n"); break; }
		case _PNG:{ strcat(buf,"Content-Type: image/png\r\n"); break; }
		default:{ strcat(buf,"Content-Type: text/plain\r\n"); }
	}
	strcat(buf,"Connection: Close\r\n");
	snprintf(temp,32,"Content-Length: %ld\r\n\r\n",st.st_size);
	strcat(buf,temp);
	
	_CallDebug(2,"Response Header:\n",buf);
	if(send(sockfd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(_LOGPATH,3,"Response200: send() Error.reason: ",strerror(errno),"\n");
		_CallDebug(3,"Response200: send() Error.reason: ",strerror(errno),"\n");
	}
	else
	{
		FILE* resource = NULL;
		i32 c = 0;

		resource = fopen(path,"rb");
		while(EOF != (c = fgetc(resource)))
		{
			if(send(sockfd,&c,1,0) <= 0)
			{
				_LogFileWrite(_LOGPATH,3,"Response200: send() Error.reason: ",strerror(errno),"\n");
				_CallDebug(3,"Response200: send() Error.reason: ",strerror(errno),"\n");
			}
		}
		if(ferror(resource))
		{ 
			_LogFileWrite(_LOGPATH,1,"Response200: I/O error when reading\n");
			_CallDebug(1,"Response200: I/O error when reading\n");
		}
		else if(feof(resource))
		{ 
			_LogFileWrite(_LOGPATH,1,"Resopnse200: End of file reached successfully\n");
			_CallDebug(1,"Resopnse200: End of file reached successfully\n");
		}
		fclose(resource);
	}					
}

void HttpResponse(i32 sockfd,const char* url)
{
	i32 path = GetRequestPath(url);
	if(_DATABASE == path)
		Response200_DB(sockfd);
	else if(_API == path)
		Response200_NF(sockfd);
	else
	{ 
		char realPath[64] = {'\0'}; 
		sprintf(realPath,"./%s/%s",_ROOTDIR,url);

		_LogFileWrite(_LOGPATH,3,"GetMethod: Real Path: ",realPath,"\n");
		_CallDebug(3,"GetMethod: Real Path: ",realPath,"\n");

		Response200(sockfd,realPath); 
	}
}

static void GetMethod(i32 sockfd,char* buf,u32 bufSize,i32 i)
{
/*	char path[64] = {'\0'}; */
	char url[128] = {'\0'};
	i32 j = 0,len = 0;
	u32 urlSize = sizeof(url);
	
	/* 获取请求路径 */
	while(isspace(buf[i++]));
	while(!isspace(buf[i]) && j < (urlSize - 1))
	{
		url[j] = buf[i];
		++i; ++j;
	}

	_LogFileWrite(_LOGPATH,3,"GetMethod: Request URL: ",url,"\n");
	_CallDebug(3,"GetMethod: Request URL: ",url,"\n");

	_LogFileWrite(_LOGPATH,1,"Request Header:\n");
	_CallDebug(1,"Request Header:\n");

	/* 读取剩下的header报文 */
	do
	{
		len = GetLine(sockfd,buf,bufSize);
		_LogFileWrite(_LOGPATH,2,buf,"\n");
		_CallDebug(2,buf,"\n");
	}while(len > 0);

	/* 通过定位?号来处理url */
	char* pos = strchr(url,'?');
	if(pos)
	{
		*pos = '\0';
		_LogFileWrite(_LOGPATH,3,"GetMethod: Position ? Request Path: ",url,"\n");
		_CallDebug(3,"GetMethod: Position ? Request Path: ",url,"\n");
	}
/*
	sprintf(path,"./%s/%s",_ROOTDIR,url);
	_LogFileWrite(_LOGPATH,3,"GetMethod: Real Path: ",path,"\n");
	_CallDebug(3,"GetMethod: Real Path: ",path,"\n");
*/
	_LogFileWrite(_LOGPATH,1,"GetMethod Response...\n");
	_CallDebug(1,"GetMethod Response...\n");

	/* 返回响应报文 */
/*	HttpResponse(sockfd,path); */
	HttpResponse(sockfd,url); 
}

static void PostMethod(i32 sockfd,char* buf,u32 bufSize)
{
	i32 len = 0;
	char contentLength[32] = {'\0'};

	_LogFileWrite(_LOGPATH,1,"Request Header:\n");
	_CallDebug(1,"Request Header:\n");
	do
	{
		_LogFileWrite(_LOGPATH,2,buf,"\n");
		_CallDebug(2,buf,"\n");

		len = GetLine(sockfd,buf,bufSize);
		if(NULL != strstr(buf,"Content-Length"))
			strcpy(contentLength,(strchr(buf,':') + 1));
	}while(len > 0);

	if(0 == GetLine(sockfd,buf,_stoi(contentLength) + 1))
	{
		_LogFileWrite(_LOGPATH,1,"PostMethod: GetLine() Error Not have body content\n");
		_CallDebug(1,"PostMethod: GetLine() Error Not have body content\n");
		return;
	}

	_LogFileWrite(_LOGPATH,1,"PostMethod Response....\n");
	_CallDebug(1,"PostMethod Response....\n");

	char nickname[24] = {'\0'};
	char email[48] = {'\0'};
	char content[1024] = {'\0'};
	char createat[24] = {'\0'};
	char* p_nickname = nickname;
	char* p_email = email;
	char* p_content = content;
	char* p_createat = createat;
	
	while('\0' != *buf && ':' != *buf)
	{ 
		*p_nickname = *buf;
		++p_nickname; ++buf;
	} 
	for(++buf;'\0' != *buf && ':' != *buf;++p_email,++buf)
		*p_email = *buf;
	for(++buf;'\0' != *buf && ':' != *buf;++p_content,++buf)
		*p_content = *buf;
	for(++buf;'\0' != *buf;++p_createat,++buf)
		*p_createat = *buf;

	_LogFileWrite(_LOGPATH,9,"PostMethod: Recv Body: ",nickname,":",email,":",content,":",createat,"\n");
	_CallDebug(9,"PostMethod: Recv Body: ",nickname,":",email,":",content,":",createat,"\n");

	struct Element el;
	MYSQL* mysql = DatabaseConnect();
  
	memset(&el,'\0',sizeof(el));
	strcpy(el.tablename,"CommentBlock");
	strcpy(el.nickname,nickname);
	strcpy(el.email,email);
	strcpy(el.content,content);
	strcpy(el.createat,createat);
	TableInsert(mysql,&el);
	DatabaseConnectClose(mysql);

	HttpResponse(sockfd,"api");
}

void HttpAccept(i32 sockfd)
{
	char buf[1024] = {'\0'};
	char method[8] = {'\0'};
	i32 len = 0;
	u32 bufSize = sizeof(buf);

	len = GetLine(sockfd,buf,bufSize);
	if(len > 0)
	{
		i32 i = 0,j = 0;
		u32 methodSize = sizeof(method);

		/* 获取请求的method */
		while(!isspace(buf[i]) && j < (methodSize - 1))
		{
			method[j] = buf[i];
			++i; ++j;
		}
		
		/* 处理对应的方法 */
		switch(GetMethodType(method))
		{
			case _GET:{ GetMethod(sockfd,buf,bufSize,i); break; }
			case _POST:{ PostMethod(sockfd,buf,bufSize); break;}
			case _PUT:{ break; }
			default:
			{
				/* 不支持的请求方法 */
				_CallDebug(3,"warning! could not accept ",method,"method\n");
				do
				{
					_LogFileWrite(_LOGPATH,2,buf,"\n");
					_CallDebug(2,buf,"\n");
					len = GetLine(sockfd,buf,bufSize);
				}while(len > 0);
			}
		}		
	}
	else{ Response400(sockfd); }					
}
