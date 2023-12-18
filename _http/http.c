#include"http.h"

/* URL路径格式化 */
static void UrlFormat(char* buf,const size_t buf_len)
{
	if(0 == buf_len){ return; }

	char url[256] = {'\0'};
	size_t i = 0,j = 0;

	if('\\' != buf[0] && '/' != buf[0]){ url[j++] = '/'; }
	for(;i < buf_len;++i)
	{
		const char ch = buf[i];
		if('\\' == ch || '/' == ch){ continue; }
		if(0 != i && ('/' == buf[i - 1] || '\\' == buf[i - 1])){
			url[j++] = '/';
		}
		url[j++] = ch;
	}
	
	for(i = 0;i < j;++i){ buf[i] = url[i]; }
	buf[i] = '\0';

	_LogFileWrite(_LOGPATH,3,"HttpServer_UrlFormat() => Format URL: ",buf,"\n");
	_CallDebug(3,"HttpServer_UrlFormat() => Format URL: ",buf,"\n");
}

/* 接收请求并将内容分类 */
static bool ReqContentReceive(int sock_fd,struct ReqContent* req_content)
{
	char buf[4096] = {'\0'};
	const size_t buf_size = sizeof(buf);
	size_t buf_len = 0;

	if(0 == (buf_len = TcpStream_StreamParseLine(sock_fd,buf,buf_size)))
	{
		_LogFileWrite(
			_LOGPATH,1,
			"HttpServer_ReqContentReceive() => TcpStream_StreamParseLine() fail\n"
		);
		_CallDebug(
			1,
			"HttpServer_ReqContentReceive() => TcpStream_StreamParseLine() fail\n"
		);
		return false;
	}	
	size_t i = 0,j = 0;
	char bodyLen[12] = {'\0'};

	while(i < buf_len && !isspace(buf[i])){
		req_content->req_method[j++] = buf[i++];
	}

	for(++i,j = 0;i < buf_len && !isspace(buf[i]);){
		req_content->req_url[j++] = buf[i++];
	}
	req_content->url_len = j;

	_LogFileWrite(_LOGPATH,1,"HttpServer_ReqContentReceive() => \nRequest Header:\n");
	_CallDebug(1,"HttpServer_ReqContentReceive() => \nRequest Header:\n");

	/* 读取剩下的header报文 */
	do
	{
		_LogFileWrite(_LOGPATH,2,buf,"\n");
		_CallDebug(2,buf,"\n");

		memset(&buf,0,buf_size);
		buf_len = TcpStream_StreamParseLine(sock_fd,buf,buf_size);
		if(NULL != strstr(buf,"Content-Length")){
			strcpy(bodyLen,(strchr(buf,':') + 1));
		}
	}while(buf_len > 0);	

	if('\0' != bodyLen[0])
	{
		/* 读取body内容 */
		if(0 == (buf_len = recv(sock_fd,req_content->req_body,(size_t)(_stoi(bodyLen) + 1),0)))
		{
			_LogFileWrite(
				_LOGPATH,1,
				"HttpServer_ReqContentReceive() => Receive Nothing body\n"
			);
			_CallDebug(
				1,
				"HttpServer_ReqContentReceive() => Receive Nothing body\n"
			);
		}
		else
		{
			_LogFileWrite(_LOGPATH,2,req_content->req_body,"\n");
			_CallDebug(2,req_content->req_body,"\n");
		}
	}
	_LogFileWrite(
		_LOGPATH,1,
		"HttpServer_ReqContentReceive() => Succeed\n"
	);
	_CallDebug(
		1,
		"HttpServer_ReqContentReceive() => Succeed\n"
	);
	return true;	
}

/* 处理HTTP的自定义格式的Body内容 */
static void BodyTackle(const char* body_buf,struct Record* record)
{
	char* p_nickname = record->nickname;
	char* p_email = record->email;
	char* p_content = record->content;
	char* p_createat = record->createat;
	
	while('\0' != *body_buf && ':' != *body_buf){ 
		*(p_nickname++) = *(body_buf++);
	} 
	*p_nickname = '\0';

	for(++body_buf;'\0' != *body_buf && ':' != *body_buf;){
		*(p_email++) = *(body_buf++);
	}
	*p_email = '\0';

	for(++body_buf;'\0' != *body_buf && ':' != *body_buf;){
		*(p_content++) = *(body_buf++);
	}
	*p_content = '\0';

	for(++body_buf;'\0' != *body_buf;){
		*(p_createat++) = *(body_buf++);
	}
	*p_createat = '\0';

	strcpy(record->tablename,_TABLENAME);

	_LogFileWrite(
		_LOGPATH,9,
		"HttpServer_BodyTackle() => \nReceive Body: \n",
		record->nickname,":",record->email,":",record->content,":",record->createat,"\n"
	);
	_CallDebug(
		9,
		"HttpServer_BodyTackle() => \nReceive Body: \n",
		record->nickname,":",record->email,":",record->content,":",record->createat,"\n"
	);
}

/* 处理GET方法 */
static void GetMethod(int sock_fd,char* url_buf,const size_t buf_len)
{
	if(0 == buf_len){ return; }

	char url[256] = {'\0'};
	size_t i = 0,url_len = 0;

	/* 获取请求路径 */
	while(i < buf_len && !isspace(url_buf[i])){
		url[url_len++] = url_buf[i++];
	}

	_LogFileWrite(_LOGPATH,3,"HttpServer_GetMethod() => Request URL: ",url,"\n");
	_CallDebug(3,"HttpServer_GetMethod() => Request URL: ",url,"\n");

	_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethod() => Response...\n");
	_CallDebug(1,"HttpServer_GetMethod() => Response...\n");

	UrlFormat(url,url_len);
	ResponseSelector(sock_fd,url); 
}

/* 处理POST方法 */
static void PostMethod(int sock_fd,char* body_buf)
{
	_LogFileWrite(_LOGPATH,1,"HttpServer_PostMethod() => Response...\n");
	_CallDebug(1,"HttpServer_PostMethod() => Response...\n");

	struct Record record;
	MYSQL* mysql = NULL;

	memset(&record,0,sizeof(struct Record));
	BodyTackle(body_buf,&record);

	if(NULL == (mysql = DatabaseConnect())){ return; }
	TableInsert(mysql,&record);
	DatabaseConnectClose(mysql);

	ResponseSelector(sock_fd,"api");
}

/* 接收HTTP请求 */
void HttpServer_Accept(int sock_fd)
{
	if(-1 == sock_fd)
	{
		_LogFileWrite(_LOGPATH,1,"HttpServer_Accept() => None apply socket\n");
		_CallDebug(1,"HttpServer_Accept() => None apply socket\n");	
		return;
	}
		
	struct ReqContent req_content;
	size_t req_content_size = sizeof(struct ReqContent);
	memset(&req_content,0,req_content_size);

	if(false == ReqContentReceive(sock_fd,&req_content)){
		return;
	}
	switch(GetMethodType(req_content.req_method))
	{
		case _GET:{ GetMethod(sock_fd,req_content.req_url,req_content.url_len); break; }
		case _POST:{ PostMethod(sock_fd,req_content.req_body); break;}
		case _PUT:{ break; }
		default:
		{
			/* 不支持的请求方法 */
			_LogFileWrite(
				_LOGPATH,3,
				"HttpServer_Accept() => warning! could not accept ",
				req_content.req_method," method\n"
			);
			_CallDebug(
				3,
				"HttpServer_Accept() => warning! could not accept ",
				req_content.req_method," method\n"
			);
			Response400(sock_fd);
		}
	}
}
