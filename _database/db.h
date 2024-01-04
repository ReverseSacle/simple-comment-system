#pragma once
#include"../mylibs.h"
#include<mysql++/mysql++.h>

// 表记录中一行的数据，处理表名
struct Record
{
	std::string nickname;
	std::string email;
	std::string email_md5;
	std::string create_at;
	std::string content;
};

class DataBase
{
private:
	mysqlpp::Connection conn;
	std::string table_name;
	std::string db_name;
	static bool table_isnull(mysqlpp::Connection& connector,std::string& db_name,std::string& table_name);
public:
	bool Connect();// 链接数据库
	bool TableInsert(Record* record);// 表记录插入
	bool GetTableRecord(std::vector<Record*>& buf);// 表记录读取
};
