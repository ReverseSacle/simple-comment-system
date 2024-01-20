#include"http.h"

/**
 * 请求url路径格式的标准化
 * 将斜杠统一为正斜杠
**/
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

/**
 * HTTP请求内容解析
 * 将请求方法，请求路径以及，body内容拆分
**/
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

/**
 * HTTP请求Body部分的解析
 * 将body内容按照前后端统一的数据格式拆分为数据库表中对应记录的属性
**/
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
	(record->id).clear();
	(record->parent_id).clear();
	(record->response_id).clear();
	(record->nick_name).clear();
	(record->mail).clear();
	(record->mail_md5).clear();
	(record->create_at).clear();
	(record->comment).clear();

	for(size_t i = 0;i < n;++i)
	{
		const char c = body_buf[i];
		if(':' == c && 7 != member_ptr){ ++member_ptr; } 
		else 
		{
			switch(member_ptr)
			{
				case 0: { record->id += c; break; }
				case 1: { record->parent_id += c; break; }
				case 2: { record->response_id += c; break; }
				case 3: { record->nick_name += c; break; }
				case 4: { record->mail += c; break; }
				case 5: { record->mail_md5 += c; break; }
				case 6: { record->create_at += c; break; }
				case 7: { record->comment += c; break; }
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

// 处理HTTP的GET方法
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

// 处理HTTP的POST方法
void HttpServer::PostMethod(int sock_fd,const std::string& body_buf)
{
	DataBase database;
	Record record;
	BodyTackle(body_buf,&record);

	// log...
	MyLibs::CallLogInfo(
		"HttpServer::PostMethod() => record:"
		"\nid: " + record.id +
		"\nparent_id: " + record.parent_id +
		"\nresponse_id: " + record.response_id +
		"\nnick_name: " + record.nick_name +
		"\nmail: " + record.mail,
		"\nmail_md5: " + record.mail_md5,
		"\ncreate_at: " + record.create_at,
		"\ncomment: " + record.comment
	);
	MyLibs::CallDebug(
		"HttpServer::PostMethod() => record:"
		"\nid: " + record.id +
		"\nparent_id: " + record.parent_id +
		"\nresponse_id: " + record.response_id +
		"\nnick_name: " + record.nick_name +
		"\nmail: " + record.mail,
		"\nmail_md5: " + record.mail_md5,
		"\ncreate_at: " + record.create_at,
		"\ncomment: " + record.comment
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

	TcpResponse::ResponseSelector(sock_fd,"/api");
}

// 接收HTTP请求报文，并处理相关操作
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
