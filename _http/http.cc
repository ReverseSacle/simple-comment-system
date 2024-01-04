#include"http.h"

std::string HttpServer::UrlFormat(std::string& url_buf)
{
	const size_t buf_len = url_buf.size();
	if(0 == buf_len){ return ""; }

	std::string url;

	if('\\' != url_buf[0] && '/' != url_buf[0]){ 
		url += '/'; 
	}
	for(size_t i = 0;i < buf_len;++i)
	{
		const char ch = url_buf[i];
		if('\\' == ch || '/' == ch){ continue; }
		if(0 != i && ('/' == url_buf[i - 1] || '\\' == url_buf[i - 1])){
			url += '/';
		}
		url += ch;
	}

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::UrlFormat() => Format URL: ",
		url
	);
	MyLibs::CallDebug(
		"HttpServer::UrlFormat() => Format URL: ",
		url
	);
	return url;
}

bool HttpServer::ReqContentReceive(int sock_fd,ReqContent* req_content)
{
	std::string buf;

	if(false == TcpStream::StreamParse_Line(sock_fd,buf))
	{
		// log...
		MyLibs::CallLogError(
			"HttpServer::ReqContentReceive() => TcpStream::StreamParse_Line() fail"
		);
		MyLibs::CallDebug(
			"HttpServer::ReqContentReceive() => TcpStream::StreamParse_Line() fail"
		);
		return false;
	}	
	size_t i = 0,buf_len = buf.size();
	char bodyLen[12] = {'\0'};

	while(i < buf_len && !isspace(buf[i])){
		req_content->req_method += buf[i++];
	}

	for(++i;i < buf_len && !isspace(buf[i]);++i){
		req_content->req_url += buf[i];
	}

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::ReqContentReceive() => \nRequest Header:"
	);
	MyLibs::CallDebug(
		"HttpServer::ReqContentReceive() => \nRequest Header:"
	);

	/* 读取剩下的header报文 */
	do
	{
		// log...
		MyLibs::CallLogInfo(buf);
		MyLibs::CallDebug(buf);

		buf.clear();
		TcpStream::StreamParse_Line(sock_fd,buf);
		if(MyLibs::StartWith(buf,"Content-Length:",i))
		{
			for(size_t j = 0;i < buf.size();++j,++i){ 
				bodyLen[j] = buf[i];
			}
		}
	}while(!buf.empty());
	std::cout << std::endl;

	if('\0' != bodyLen[0])
	{
		char body_buf[2048] = {'\0'};
		/* 读取body内容 */
		buf_len = recv(
			sock_fd,body_buf,
			(size_t)(MyLibs::_stoi(bodyLen)) + 1,
			0
		);
		if(0 == buf_len)
		{
			// log...
			MyLibs::CallLogInfo(
				"HttpServer::ReqContentReceive() => Receive Nothing body"
			);
			MyLibs::CallDebug(
				"HttpServer::ReqContentReceive() => Receive Nothing body"
			);
		}
		else
		{
			for(size_t i = 0;i < buf_len;++i)
			{
				if(isspace(body_buf[i])){ continue; }
				req_content->req_body += body_buf[i];
			}
		}
	}
	// log...
	MyLibs::CallLogInfo(
		"HttpServer::ReqContentReceive() => Succeed"
	);
	MyLibs::CallDebug(
		"HttpServer::ReqContentReceive() => Succeed"
	);
	return true;	
}

void HttpServer::BodyTackle(const std::string& body_buf,Record* record)
{
	const size_t n = body_buf.size();
	if(0 == n)
	{ 
		// log...
		MyLibs::CallLogInfo(
			"HttpServer::BodyTackle() => Receive Nothing Body"
		);
		MyLibs::CallDebug(
			"HttpServer::BodyTackle() => Receive Nothing Body"
		);
		return; 
	}

	int member_ptr = 0;
	(record->nickname).clear();
	(record->email).clear();
	(record->email_md5).clear();
	(record->create_at).clear();
	(record->content).clear();
	
	for(size_t i = 0;i < n;++i)
	{
		const char c = body_buf[i];
		if(':' == c && 4 != member_ptr){ ++member_ptr; } 
		else 
		{
			switch(member_ptr)
			{
				case 0: { record->nickname += c; break; }
				case 1: { record->email += c; break; }
				case 2: { record->email_md5 += c; break; }
				case 3: { record->create_at += c; break; }
				case 4: { record->content += c; break; }
			}
		}
	}

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::BodyTackle() => Succeed"
	);
	MyLibs::CallDebug(
		"HttpServer::BodyTackle() => Succeed"
	);
}

void HttpServer::GetMethod(int sock_fd,const std::string& url_buf)
{
	const size_t buf_len = url_buf.size();
	if(0 == buf_len){ return; }

	std::string url;

	/* 获取请求路径 */
	for(size_t i = 0;i < buf_len && !isspace(url_buf[i]);++i){
		url += url_buf[i];
	}

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::GetMethod() => Request URL: ",
		url
	);
	MyLibs::CallDebug(
		"HttpServer::GetMethod() => Request URL: ",
		url
	);
	// log...
	MyLibs::CallLogInfo("HttpServer::GetMethod() => Response...");
	MyLibs::CallDebug("HttpServer::GetMethod() => Response...");

	TcpResponse::ResponseSelector(sock_fd,UrlFormat(url)); 
}

void HttpServer::PostMethod(int sock_fd,const std::string& body_buf)
{
	DataBase database;
	Record record;
	BodyTackle(body_buf,&record);

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::PostMethod() => record:"
		"\nnickname: " + record.nickname +
		"\nemail: " + record.email,
		"\nemail_md5: " + record.email_md5,
		"\ncreate_at: " + record.create_at,
		"\ncontent: " + record.content
	);
	MyLibs::CallDebug(
		"HttpServer::PostMethod() => record:"
		"\nnickname: " + record.nickname +
		"\nemail: " + record.email,
		"\nemail_md5: " + record.email_md5,
		"\ncreate_at: " + record.create_at,
		"\ncontent: " + record.content
	);

	if(false == database.Connect())
	{ 
		TcpResponse::Response400(sock_fd);
		return; 
	}
	database.TableInsert(&record);

	// log...
	MyLibs::CallLogInfo("HttpServer::PostMethod() => Response...");
	MyLibs::CallDebug("HttpServer::PostMethod() => Response...");

	TcpResponse::ResponseSelector(sock_fd,"api");
}

void HttpServer::HttpAccept(int sock_fd)
{
	if(-1 == sock_fd)
	{
		// log...
		MyLibs::CallLogError(
			"HttpServer::Accept() => None apply socket"
		);	
		MyLibs::CallDebug(
			"HttpServer::Accept() => None apply socket"
		);
		return;
	}
		
	struct ReqContent req_content;

	if(false == ReqContentReceive(sock_fd,&req_content)){
		return;
	}
	switch(TcpResponse::GetMethodType(req_content.req_method))
	{
		case RequestMethodType::_GET:{ GetMethod(sock_fd,req_content.req_url); break; }
		case RequestMethodType::_POST:{ PostMethod(sock_fd,req_content.req_body); break; }
		case RequestMethodType::_PUT:{ break; }
		default:
		{
			/* 不支持的请求方法 */
			// log...
			MyLibs::CallLogInfo(
				"HttpServer::Accept() => Not support for current ",
				req_content.req_method,
				" request method"
			);
			MyLibs::CallDebug(
				"HttpServer::Accept() => Not support for current ",
				req_content.req_method,
				" request method"
			);
			TcpResponse::Response400(sock_fd);
		}
	}
}
