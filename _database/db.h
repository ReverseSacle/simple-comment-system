#pragma once
#include<mysql/mysql.h>
#include"../mylibs.h"

#define _HOST "127.0.0.1"
#define _USER "root"
#define _PASSWORD "@Killl611"
#define _DBNAME "CommentSystem"
#define _TABLENAME "CommentBlock"

struct Record
{
	char tablename[16];
	char nickname[24];
	char email[48];
	char content[1024];
	char createat[24];
};

MYSQL* DatabaseConnect();
inline void DatabaseConnectClose(MYSQL* mysql);
void TableInsert(MYSQL* mysql,const struct Record* el); 
MYSQL_RES* GetTableRecordStart(MYSQL* mysql,const char* tablename);
inline void GetTableRecordEnd(MYSQL_RES* res);
