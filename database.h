#pragma once
#include<mysql/mysql.h>
#include"_clibs.h"

#define HOST "127.0.0.1"
#define USER "root"
#define PASSWORD "@Killl611"
#define DBNAME "CommentSystem"

struct Element
{
	char tablename[16];
	char nickname[24];
	char email[48];
	char content[1024];
	char createat[24];
};

MYSQL* DatabaseConnect();
inline void DatabaseConnectClose(MYSQL* mysql);
void TableInsert(MYSQL* mysql,const struct Element* el); 
MYSQL_RES* GetTableElementStart(MYSQL* mysql,const char* tableName);
inline void GetTableElementEnd(MYSQL_RES* res);