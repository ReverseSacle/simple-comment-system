#pragma once
#include"../mylibs.h"
#include<mysql++/mysql++.h>

// 数据库中的记录
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
	std::string table_name;// 数据库的表名
	std::string db_name;// 数据库对应表所属的库名
public:
	bool Connect();
	bool TableIsNull();
	int TableRecordCount();
	int GetTableParentRecordNum();
	bool TableInsert(Record* record);
	bool GetTableParentRecord(std::vector<Record*>& buf);
	bool GetTableChildRecord(std::vector<Record*>& buf);
};
