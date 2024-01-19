#pragma once
#include"../mylibs.h"
#include<mysql++/mysql++.h>

// 表记录中一行的数据，处理表名
struct Record
{
	std::string id;
	std::string parent_id;
	std::string response_id;
	std::string nick_name;
	std::string mail;
	std::string mail_md5;
	std::string create_at;
	std::string comment;
};

class DataBase
{
private:
	mysqlpp::Connection conn;
	std::string table_name;
	std::string db_name;
public:
	bool Connect();// 链接数据库
	bool TableIsNull();
	int TableRecordCount();
	int GetTableParentRecordNum();
	bool TableInsert(Record* record);// 表记录插入
	bool GetTableParentRecord(std::vector<Record*>& buf);// 表记录中的主评论记录读取
	bool GetTableChildRecord(std::vector<Record*>& buf);// 表记录中的回复评论记录读取
};
