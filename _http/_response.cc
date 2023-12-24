#include"_response.h"

int TcpResponse::GetMethodType(const std::string& method_buf)
{
	const size_t buf_len = method_buf.size();
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

	for(size_t i = 0;i < buf_len;++i)
	{
		const char ch = toupper(method_buf[i]);
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
				// log..
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => GET"
				);
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => GET"
				);
				return _GET;
			}
			if('\0' == *_put)
			{
				// log..
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => PUT"
				);
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => PUT"
				);
				return _PUT;
			}
			break;
		}
		case 4:
		{
			if('\0' == *_post)
			{
				// log..
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => POST"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => POST"
				); 
				return _POST;
			}
			if('\0' == *_head)
			{
				// log..
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => HEAD"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => HEAD"
				); 
				return _HEAD;
			}
			break;
		} 
		case 5:
		{
			if('\0' == *_patch)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => PATCH"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => PATCH"
				); 
				return _PATCH;
			}
			if('\0' == *_trace)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => TRACE"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => TRACE"
				); 
				return _TRACE;
			}
			break;
		}
		case 6:
		{
			if('\0' == *_delete)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => DELETE"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => DELETE"
				); 
				return _DELETE;
			}
			break;
		}
		case 7:
		{
			if('\0' == *_options)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => OPTIONS"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => OPTIONS"
				); 
				return _OPTIONS;
			}
			if('\0' == *_connect)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetMethodType() => CONNECT"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetMethodType() => CONNECT"
				); 
				return _CONNECT;
			}
			break;
		}
	}
	// log...
	MyLibs::CallLogInfo("TcpResponse::GetMethodType() => UNKNOW");
	MyLibs::CallDebug("TcpResponse::GetMethodType() => UNKNOW");
	return -1;
}

int TcpResponse::GetFileType(const std::string& filename_buf)
{
	const size_t buf_len = filename_buf.size();
	if(0 == buf_len){ return -1; }

	std::string type;

	for(size_t i = 0;i < buf_len;++i)
	{
		const char ch = filename_buf[i];
		if('/' == ch || '.' == ch){ type.clear(); }
		else{ type += ch; }
	}

	if(!type.empty())
	{
		const char* _html = "HTML";
		const char* _js = "JS";
		const char* _css = "CSS";
		const char* _png = "PNG";
		const size_t type_len = type.size();

		for(size_t i = 0;i < type_len;++i)
		{
			const char c = toupper(type[i]);
			if('\0' != *_html && *_html == c){ ++_html; }
			if('\0' != *_js && *_js == c){ ++_js; }
			if('\0' != *_css && *_css == c){ ++_css; }
			if('\0' != *_png && *_png == c){ ++_png; }
		}

		if(4 == type_len && '\0' == *_html)
		{
			// log...
			MyLibs::CallLogInfo(
				"TcpResponse::GetFileType() => HTML"
			); 
			MyLibs::CallDebug(
				"TcpResponse::GetFileType() => HTML"
			); 
			return _HTML;
		}

		if(2 == type_len && '\0' == *_js)
		{
			// log...
			MyLibs::CallLogInfo(
				"TcpResponse::GetFileType() => CSS"
			); 
			MyLibs::CallDebug(
				"TcpResponse::GetFileType() => CSS"
			); 
			return _JS;
		}

		if(3 == type_len)
		{
			if('\0' == *_css)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetFileType() => JS"
				); 
				MyLibs::CallDebug(
					"TcpResponse::GetFileType() => JS"
				); 
				return _CSS;
			}

			if('\0' == *_png)
			{
				// log...
				MyLibs::CallLogInfo(
					"TcpResponse::GetFileType() => PNG"
				);
				MyLibs::CallDebug(
					"TcpResponse::GetFileType() => PNG"
				);
				return _PNG;
			}
		}
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::GetFileType() => UNKNOW"
	);
	MyLibs::CallDebug(
		"TcpResponse::GetFileType() => UNKNOW"
	);
	return -1;
}

int TcpResponse::GetPathType(const std::string& path_buf)
{
	const size_t buf_len = path_buf.size();
	if(0 == buf_len){ return -1; }

	const char* db = "DATABASE";
	const char* api = "API";

	for(size_t i = 0;i < buf_len;++i)
	{
		const char c = toupper(path_buf[i]);
		if('\0' != *db && *db == c){ ++db; }
		if('\0' != *api && *api == c){ ++api; }
	}

	if(9 == buf_len && '\0' == *db)
	{
		// log...
		MyLibs::CallLogInfo("TcpResponse::GetPathType() => DATABASE");
		MyLibs::CallDebug("TcpResponse::GetPathType() => DATABASE");
		return _DATABASE;
	}

	if(3 == buf_len && '\0' == *api)
	{
		// log...
		MyLibs::CallLogInfo(
			"TcpResponse::GetPathType() => API"
		);
		MyLibs::CallDebug(
			"TcpResponse::GetPathType() => API"
		);
		return _API;
	}

	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::GetPathType() => NORMAL"
	);
	MyLibs::CallDebug(
		"TcpResponse::GetPathType() => NORMAL"
	);
	return -1;	
}

void TcpResponse::Response400(int sock_fd)
{
	std::string buf;

	buf += "HTTP/";
	buf += _HTTPVERSION;
	buf += " 400 BAD REQUEST\r\n\r\n";

	if(send(sock_fd,&buf[0],buf.size(),0) <= 0)
	{
		// log...
		MyLibs::CallLogError(
			"TcpResponse::Response400() => send() fail.\nreason: ",
			strerror(errno)
		);
		MyLibs::CallDebug(
			"TcpResponse::Response400() => send() fail.\nreason: ",
			strerror(errno)
		);
		return;
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::Response400() => Succeed\nResponse Header:\n",
		buf
	);
	MyLibs::CallDebug(
		"TcpResponse::Response400() => Succeed\nResponse Header:\n",
		buf
	);
}

void TcpResponse::Response404(int sock_fd)
{
	std::string buf;

	buf += "HTTP/";
	buf += _HTTPVERSION;
	buf += " 404 NOT FOUND\r\n\r\n";

	if(send(sock_fd,&buf[0],buf.size(),0) <= 0)
	{
		// log...
		MyLibs::CallLogError(
			"TcpResponse::Response404() => send() fail.\nreason: ",
			strerror(errno)
		);
		MyLibs::CallDebug(
			"TcpResponse::Response404() => send() fail.\nreason: ",
			strerror(errno)
		);
		return;
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::Response404() => Succeed\nResponse Header:\n",
		buf
	);
	MyLibs::CallDebug(
		"TcpResponse::Response404() => Succeed\nResponse Header:\n",
		buf 
	);
}

void TcpResponse::Response200_NF(int sock_fd)
{
	std::string buf;

	buf += "HTTP/";
	buf += _HTTPVERSION;
	buf += " 200 OK\r\n";
	buf += "Server: Linux\r\n";
	buf += "Connection: Close\r\n\r\n";
	
	if(send(sock_fd,&buf[0],buf.size(),0) <= 0)
	{
		// log....
		MyLibs::CallLogError(
			"TcpResponse::Response200_NF() => send() fail.\nreason: ",
			strerror(errno)
		);
		MyLibs::CallDebug(
			"TcpResponse::Response200_NF() => send() fail.\nreason: ",
			strerror(errno)
		);
		return;
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::Response200_NF() => Succeed\nResponse Header:\n",
		buf
	); 
	MyLibs::CallDebug(
		"TcpResponse::Response200_NF() => Succeed\nResponse Header:\n",
		buf
	); 
}

void TcpResponse::Response200_DB(int sock_fd)
{
	std::vector<Record> records;
	DataBase database;
	std::string buf;
	std::string body;
	
	buf += "HTTP/1.0 200 OK\r\n";
	buf += "Server: Linux\r\n";
	buf += "Content-Type: text/plain\r\n";
	buf += "Connection: Close\r\n";
	
	if(false == database.Connect()){ return; }
	if(false == database.GetTableRecord(records)){ return; }
	for(auto& r: records){
		body += (r.nickname + ":" + r.email + ":" + r.content + ":" + r.createat + ",");
	}

	const size_t bodyLen = body.size();
	buf += "Content-Length: ";
	buf += std::to_string(bodyLen);
	buf += "\r\n\r\n";
	
	if(send(sock_fd,&buf[0],buf.size(),0) <= 0)
	{
		// log...
		MyLibs::CallLogError(
			"TcpResponse::Response200_DB() => send() fail.\nreason: ",
			strerror(errno)
		);
		MyLibs::CallDebug(
			"TcpResponse::Response200_DB() => send() fail.\nreason: ",
			strerror(errno)
		);
		return;
	}

	if(0 != bodyLen)
	{
		if(send(sock_fd,&body[0],bodyLen,0) <= 0)
		{
			// log...	
			MyLibs::CallLogError(
				"TcpResponse::Response200_DB() => body send() fail.\nreason: ",
				strerror(errno)
			);
			MyLibs::CallDebug(
				"TcpResponse::Response200_DB() => body send() fail.\nreason: ",
				strerror(errno)
			);
			return;
		}
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::Response200_DB() => Succeed\nResponse Header:\n",
		buf
	);
	MyLibs::CallDebug(
		"TcpResponse::Response200_DB() => Succeed\nResponse Header:\n",
		buf
	);
}

void TcpResponse::Response200(int sock_fd,const std::string& path,const off_t f_size)
{
	std::string buf;

	buf += "HTTP/";
	buf += _HTTPVERSION;
	buf += " 200 OK\r\n";
	buf += "Server: Linux\r\n";
	switch(GetFileType(path))
	{
		case _HTML: { buf += "Content-Type: text/html\r\n"; break; }
		case _CSS: { buf += "Content-Type: text/css\r\n"; break; }
		case _JS: { buf += "Content-Type: text/javascript\r\n"; break; }
		case _PNG: { buf += "Content-Type: image/png\r\n"; break; }
		default: { buf += "Content-Type: text/plain\r\n"; }
	}
	buf += "Connection: Close\r\n";
	buf += "Content-Length: ";
	buf += std::to_string(f_size);
	buf += "\r\n\r\n";

	if(send(sock_fd,&buf[0],buf.size(),0) <= 0)
	{
		// log...
		MyLibs::CallLogError(
			"TcpResponse::Response200() => send() fail.\nreason: ",
			strerror(errno)
		);
		MyLibs::CallDebug(
			"TcpResponse::Response200() => send() fail.\nreason: ",
			strerror(errno)
		);
		return;
	}
	std::ifstream resource(path,std::ios::binary);

	if(!resource.is_open())
	{
		// log...
		MyLibs::CallLogError(
			"HttpServer::Response200() => file open fail"
		);
		MyLibs::CallDebug(
			"HttpServer::Response200() => file open fail"
		);
		return;
	}

	int c = 0;
	while(EOF != (c = resource.get()))
	{
		if(send(sock_fd,&c,1,0) <= 0)
		{
			// log...
			MyLibs::CallLogError(
				"HttpServer::Response200() => file read fail"
			);
			MyLibs::CallDebug(
				"HttpServer::Response200() => file read fail"
			);
			return;
		}
	}
		
	if(resource.eof())
	{
		// log...
		MyLibs::CallLogInfo(
			"HttpServer::Response200() => End of reading file"
		);
		MyLibs::CallDebug(
			"HttpServer::Response200() => End of reading file"
		);
	}
	else
	{
		// log...
		MyLibs::CallLogError(
			"HttpServer::Response200() => I/O error when reading"
		);
		MyLibs::CallDebug(
			"HttpServer::Response200() => I/O error when reading"
		);
		return;
	}
	// log...
	MyLibs::CallLogInfo(
		"TcpResponse::Response200() => Succeed\nResponse Header:\n",
		buf
	);
	MyLibs::CallDebug(
		"TcpResponse::Response200() => Succeed\nResponse Header:\n",
		buf
	);
}

void TcpResponse::ResponseSelector(int sock_fd,const std::string& url_buf)
{
	switch(GetPathType(url_buf))
	{
		case _DATABASE: { Response200_DB(sock_fd); break; }
		case _API: { Response200_NF(sock_fd); break; }
		default: 
		{
			std::string buf;
			struct stat st;

			buf += "./";
			buf += MyLibs::GetRootDir();
			buf += url_buf;

			// log...
			MyLibs::CallLogInfo(
				"TcpResponse::ResponseSelector() => Index_Path: ",
				buf
			);
			MyLibs::CallDebug(
				"TcpResponse::ResponseSelector() => Index_Path: ",
				buf
			);

			if(-1 == stat(&buf[0],&st)){ Response404(sock_fd); }
			else{ Response200(sock_fd,buf,st.st_size); }
		}
	}
}

