#include"_response.h"

/* 获取HTTP请求方法 */
int GetMethodType(const char* method_buf)
{
	const size_t buf_len = strlen(method_buf);
	if(0 == buf_len){ return -1; }

	const char* _get = "GET";
	const char* _post = "POST";
	const char* _put = "PUT";
	const char* _delete = "DELETE";
	const char* _head = "HEAD";
	const char* _options = "OPTIONS";
	const char* _patch = "PATCH";
	const char* _connect = "CONNECT";
	const char* _trace = "TRACE";
	size_t i = 0;

	while(i < buf_len)
	{
		const char ch = toupper(method_buf[i++]);
		if('\0' != *_get && ch == *_get){ ++_get; }
		if('\0' != *_post && ch == *_post){ ++_post; }
		if('\0' != *_put && ch == *_put){ ++_put; }
		if('\0' != *_delete && ch == *_delete){ ++_delete; }
		if('\0' != *_head && ch == *_head){ ++_head; }
		if('\0' != *_options && ch == *_options){ ++_options; }
		if('\0' != *_patch && ch == *_patch){ ++_patch; }
		if('\0' != *_connect && ch == *_connect){ ++_connect; }
		if('\0' != *_trace && ch == *_trace){ ++_trace; }
	}

	switch(buf_len)
	{
		case 3:
		{
			if('\0' == *_get)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => GET\n");
				_CallDebug(1,"HttpServer_GetMethodType() => GET\n");
				return _GET;
			}
			if('\0' == *_put)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => PUT\n");
				_CallDebug(1,"HttpServer_GetMethodType() => PUT\n");
				return _PUT;
			}
			break;
		}
		case 4:
		{
			if('\0' == *_post)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => POST\n");
				_CallDebug(1,"HttpServer_GetMethodType() => POST\n"); 
				return _POST;
			}
			if('\0' == *_head)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => HEAD\n");
				_CallDebug(1,"HttpServer_GetMethodType() => HEAD\n"); 
				return _HEAD;
			}
			break;
		} 
		case 5:
		{
			if('\0' == *_patch)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => PATCH\n");
				_CallDebug(1,"HttpServer_GetMethodType() => PATCH\n"); 
				return _PATCH;
			}
			if('\0' == *_trace)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => TRACE\n");
				_CallDebug(1,"HttpServer_GetMethodType() => TRACE\n"); 
				return _TRACE;
			}
			break;
		}
		case 6:
		{
			if('\0' == *_delete)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => DELETE\n");
				_CallDebug(1,"HttpServer_GetMethodType() => DELETE\n"); 
				return _DELETE;
			}
			break;
		}
		case 7:
		{
			if('\0' == *_options)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => OPTIONS\n");
				_CallDebug(1,"HttpServer_GetMethodType() => OPTIONS\n"); 
				return _OPTIONS;
			}
			if('\0' == *_connect)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => CONNECT\n");
				_CallDebug(1,"HttpServer_GetMethodType() => CONNECT\n"); 
				return _CONNECT;
			}
			break;
		}
	}
	_LogFileWrite(_LOGPATH,1,"HttpServer_GetMethodType() => UNKNOW\n");
	_CallDebug(1,"HttpServer_GetMethodType() => UNKNOW\n");
	return -1;
}

/* 获取请求路径文件格式 */
int GetFileType(const char* filename_buf)
{
	const size_t buf_len = strlen(filename_buf);
	if(0 == buf_len){ return -1; }

	char type[256] = {'\0'};
	size_t i = 0,index = 0;

	while(i < buf_len)
	{
		const char ch = filename_buf[i++];
		if('/' == ch || '.' == ch){ index = 0; }
		else{ type[index++] = ch; }
	}
	type[index] = '\0';

	if(0 != index)
	{
		const char* _html = "HTML";
		const char* _js = "JS";
		const char* _css = "CSS";
		const char* _png = "PNG";
		size_t i = 0,type_len = index;

		for(i = 0;i < type_len;++i)
		{
			const char c = toupper(type[i]);
			if('\0' != *_html && *_html == c){ ++_html; }
			if('\0' != *_js && *_js == c){ ++_js; }
			if('\0' != *_css && *_css == c){ ++_css; }
			if('\0' != *_png && *_png == c){ ++_png; }
		}
		if(4 == type_len && '\0' == *_html)
		{
			_LogFileWrite(_LOGPATH,1,"HttpServer_GetFileType() => HTML\n");
			_CallDebug(1,"HttpServer_GetFileType() => HTML\n"); 
			return _HTML;
		}

		if(2 == type_len && '\0' == *_js)
		{
			_LogFileWrite(_LOGPATH,1,"HttpServer_GetFileType() => CSS\n");
			_CallDebug(1,"HttpServer_GetFileType() => CSS\n"); 
			return _JS;
		}
		if(3 == type_len)
		{
			if('\0' == *_css)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetFileType() => JS\n");
				_CallDebug(1,"HttpServer_GetFileType() => JS\n"); 
				return _CSS;
			}

			if('\0' == *_png)
			{
				_LogFileWrite(_LOGPATH,1,"HttpServer_GetFileType() => PNG\n");
				_CallDebug(1,"HttpServer_GetFileType() => PNG\n");
				return _PNG;
			}
		}
	}
	_LogFileWrite(_LOGPATH,1,"HttpServer_GetFileType() => UNKNOW\n");
	_CallDebug(1,"HttpServer_GetFileType() => UNKNOW\n");
	return -1;
}

/* 获取请求路径应用类型 */
int GetPathType(const char* path_buf)
{
	size_t buf_len = strlen(path_buf);
	if(0 == buf_len){ return -1; }

	const char* db = "DATABASE";
	const char* api = "API";
	size_t i = 0;

	while(i < buf_len)
	{
		const char c = toupper(path_buf[i++]);
		if('\0' != *db && *db == c){ ++db; }
		if('\0' != *api && *api == c){ ++api; }
	}

	if(9 == buf_len && '\0' == *db)
	{
		_LogFileWrite(_LOGPATH,1,"HttpServer_GetPathType() => DATABASE\n");
		_CallDebug(1,"HttpServer_GetPathType() => DATABASE\n");
		return _DATABASE;
	}

	if(3 == buf_len && '\0' == *api)
	{
		_LogFileWrite(_LOGPATH,1,"HttpServer_GetPathType() => API\n");
		_CallDebug(1,"HttpServer_GetPathType() => API\n");
		return _API;
	}

	_LogFileWrite(_LOGPATH,1,"HttpServer_GetPathType() => NORMAL\n");
	_CallDebug(1,"HttpServer_GetPathType() => NORMAL\n");
	return -1;	
}

void Response400(int sock_fd)
{
	char buf[1024] = {0};

	strcpy(buf,"HTTP/");
	strcat(buf,_HTTPVERSION);
	strcat(buf," 400 BAD REQUEST\r\n\r\n");

	if(send(sock_fd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(
			_LOGPATH,3,
			"HttpServer_Response400() => send() fail.reason: ",strerror(errno),"\n"
		);
		_CallDebug(
			3,
			"HttpServer_Response400() => send() fail.reason: ",strerror(errno),"\n"
		);
	}
}

static void Response404(int sock_fd)
{
	char buf[1024] = {0};

	strcpy(buf,"HTTP/");
	strcat(buf,_HTTPVERSION);
	strcat(buf," 404 NOT FOUND\r\n\r\n");

	if(send(sock_fd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(
			_LOGPATH,3,
			"HttpServer_Response404() => send() fail.reason: ",strerror(errno),"\n"
		);
		_CallDebug(
			3,
			"HttpServer_Response404() => send() fail.reason: ",strerror(errno),"\n"
		);
	}
}

/* 非GET方法的200响应 */
static void Response200_NF(int sock_fd)
{
	char buf[1024] = {0};

	strcpy(buf,"HTTP/");
	strcat(buf,_HTTPVERSION);
	strcat(buf," 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	strcat(buf,"Connection: Close\r\n\r\n");
	
	if(send(sock_fd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(
			_LOGPATH,3,
			"HttpServer_Response200_NF() => send() fail.reason: ",strerror(errno),"\n"
		);
		_CallDebug(
			3,
			"HttpServer_Response200_NF() => send() fail.reason: ",strerror(errno),"\n"
		);
	}
	else { _CallDebug(2,"HttpServer_Response200_NF() => \nResponse Header:\n",buf); }
}

/* 数据库处理的200响应 */
static void Response200_DB(int sock_fd)
{
	char buf[2048] = {'\0'};
	char body[2048] = {'\0'};
	char temp[256] = {'\0'};
	MYSQL* mysql = NULL;

	if(NULL == (mysql = DatabaseConnect())){ return; }

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	strcat(buf,"Content-Type: text/plain\r\n");
	strcat(buf,"Connection: Close\r\n");

	MYSQL_RES* res = GetTableRecordStart(mysql,_TABLENAME);
	if(NULL != res)
	{
		MYSQL_ROW row;
		while((row = mysql_fetch_row(res)))
		{
			sprintf(temp,"%s:%s:%s:%s,",row[0],row[1],row[2],row[3]);
			strcat(body,temp);
		}
		GetTableRecordEnd(res);
	}
	DatabaseConnectClose(mysql);

	int bodyLen = strlen(body);
	sprintf(temp,"Content-Length: %d\r\n\r\n",bodyLen);
	strcat(buf,temp);

	_LogFileWrite(_LOGPATH,2,"HttpServer_HTTP200_DB() => \nResponse Header:\n",buf);
	_CallDebug(2,"HttpServer_HTTP200_DB() => \nResponse Header:\n",buf);

	if(send(sock_fd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(
			_LOGPATH,3,
			"HttpServer_Response200_DB() => send() fail.\nreason: \n",strerror(errno),"\n"
		);
		_CallDebug(
			3,
			"HttpServer_Response200_DB() => send() fail.\nreason: \n",strerror(errno),"\n"
		);
	}
	else if(0 != bodyLen)
	{
		if(send(sock_fd,body,bodyLen,0) <= 0)
		{
			_LogFileWrite(
				_LOGPATH,3,
				"HttpServer_Response200_DB() => send() fail.\nreason: \n",strerror(errno),"\n"
			);
			_CallDebug(
				3,
				"HttpServer_Response200_DB() => send() fail.\nreason: \n",strerror(errno),"\n"
			);
		}
	}
}


/* GET方法的200响应 */
static void Response200(int sock_fd,const char* path,const off_t f_size)
{
	char buf[1024] = {'\0'};
	char temp[256] = {'\0'};

	strcpy(buf,"HTTP/");
	strcat(buf,_HTTPVERSION);
	strcat(buf," 200 OK\r\n");
	strcat(buf,"Server: Linux\r\n");
	switch(GetFileType(path))
	{
		case _HTML: { strcat(buf,"Content-Type: text/html\r\n"); break; }
		case _CSS: { strcat(buf,"Content-Type: text/css\r\n"); break; }
		case _JS: { strcat(buf,"Content-Type: text/javascript\r\n"); break; }
		case _PNG: { strcat(buf,"Content-Type: image/png\r\n"); break; }
		default: { strcat(buf,"Content-Type: text/plain\r\n"); }
	}
	strcat(buf,"Connection: Close\r\n");
	sprintf(temp,"Content-Length: %ld\r\n\r\n",f_size);
	strcat(buf,temp);

	_CallDebug(2,"HttpServer_Response200() => \nResponse Header:\n",buf);
	if(send(sock_fd,buf,strlen(buf),0) <= 0)
	{
		_LogFileWrite(
			_LOGPATH,3,
			"HttpServer_Response200() => send() fail.reason: ",strerror(errno),"\n"
		);
		_CallDebug(
			3,
			"HttpServer_Response200() => send() fail.reason: ",strerror(errno),"\n"
		);
	}
	else
	{
		FILE* resource = NULL;
		int c = 0;

		resource = fopen(path,"rb");
		while(EOF != (c = fgetc(resource)))
		{
			if(send(sock_fd,&c,1,0) <= 0)
			{
				_LogFileWrite(
					_LOGPATH,3,
					"HttpServer_Response200() => send() Error.reason: ",strerror(errno),"\n"
				);
				_CallDebug(
					3,
					"HttpServer_Response200() => send() Error.reason: ",strerror(errno),"\n"
				);
			}
		}
		if(ferror(resource))
		{ 
			_LogFileWrite(
				_LOGPATH,1,
				"HttpServer_Response200() => I/O error when reading\n"
			);
			_CallDebug(
				1,
				"HttpServer_Response200() => I/O error when reading\n"
			);
		}
		else if(feof(resource))
		{ 
			_LogFileWrite(
				_LOGPATH,1,
				"HttpServer_Response200() => End of file reached successfully\n"
			);
			_CallDebug(
				1,
				"HttpServer_Response200() => End of file reached successfully\n"
			);
		}
		fclose(resource);
	} 	
}

/* 报文响应选择器 */
void ResponseSelector(int sock_fd,const char* url_buf)
{
	switch(GetPathType(url_buf))
	{
		case _DATABASE: { Response200_DB(sock_fd); break; }
		case _API: { Response200_NF(sock_fd); break; }
		default: 
		{
			char buf[1024] = {'\0'};
			strcpy(buf,"./");
			strcat(buf,_ROOTDIR);
			strcat(buf,url_buf);

			_LogFileWrite(
				_LOGPATH,3,
				"HttpServer_ResponseSelector() => Index_Path: ",buf,"\n"
			);
			_CallDebug(
				3,
				"HttpServer_ResponseSelector() => Index_Path: ",buf,"\n"
			);

			struct stat st;
			if(-1 == stat(buf,&st)){ Response404(sock_fd); }
			else{ Response200(sock_fd,buf,st.st_size); }
		}
	}
}
