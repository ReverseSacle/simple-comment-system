#pragma once
#include<mysql++/mysql++.h>
#include"../mylibs.h"

#define _HOST "127.0.0.1"
#define _USER "root"
#define _PASSWORD "@Killl611"
#define _DBNAME "CommentSystem"
#define _TABLENAME "CommentBlock"

// 表记录中一行的数据，处理表名
struct Record
{
	std::string tablename;// 表名
	std::string nickname;
	std::string email;
	std::string content;
	std::string createat;
};

class DataBase
{
private:
	mysqlpp::Connection conn;
public:
	bool Connect();// 链接数据库
	bool TableInsert(Record* record);// 表记录插入
	bool GetTableRecord(std::vector<Record>& buf);// 表记录读取
};


