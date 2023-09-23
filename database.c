#include"database.h"

MYSQL* DatabaseConnect()
{
	MYSQL* mysql = mysql_init(NULL);
	if(NULL == mysql)
	{
		_LogFileWrite(_LOGPATH,1,"DatabaseConnect: mysql_init() Error\n");
		_CallDebug(1,"DatabaseConnect: mysql_init() Error\n");
		return NULL;
	}
	
	mysql = mysql_real_connect(mysql,HOST,USER,PASSWORD,DBNAME,0,NULL,0);
	if(NULL == mysql)
	{
		_LogFileWrite(_LOGPATH,1,"DatabaseConnect: mysql_real_connect() Error\n");
		_CallDebug(1,"DatabaseConnect: mysql_real_connect() Error\n");
		return NULL;
	}

	_LogFileWrite(_LOGPATH,1,"DatabaseConnect Succeed\n");
	_CallDebug(1,"DatabaseConnect Succeed\n");
	return mysql;
}

inline void DatabaseConnectClose(MYSQL* mysql)
{
	mysql_close(mysql);
	_LogFileWrite(_LOGPATH,1,"DatabaseConnect Close\n");
	_CallDebug(1,"DatabaseConnect Close\n");
}

void TableInsert(MYSQL* mysql,const struct Element* el)
{
	char buf[1024] = {'\0'};
	
	sprintf(buf,"INSERT INTO %s(NickName,Email,Content,CreateAt)\nVALUES('%s','%s','%s','%s')",el->tablename,el->nickname,el->email,el->content,el->createat);

	_LogFileWrite(_LOGPATH,3,"Database Query:\n",buf,"\n");
	_CallDebug(3,"Database Query:\n",buf,"\n");
	if(0 != mysql_query(mysql,buf))
	{
		_LogFileWrite(_LOGPATH,3,"TableInsert: mysql_query() Error.reason: ",mysql_error(mysql),"\n");
		_CallDebug(3,"TableInsert: mysql_query() Error.reason: ",mysql_error(mysql),"\n");
		return;
	}
	_LogFileWrite(_LOGPATH,1,"TableInsert Succeed\n");
	_CallDebug(1,"TableInsert Succeed\n");		
}

MYSQL_RES* GetTableElementStart(MYSQL* mysql,const char* tableName)
{
	char temp[64] = {'\0'};

	sprintf(temp,"SELECT * FROM %s ORDER BY CreateAT ASC",tableName);
	if(0 != mysql_query(mysql,temp))
	{
		_LogFileWrite(_LOGPATH,1,"GetTableElement: mysql_query() Error\n");
		_CallDebug(1,"GetTableElement: mysql_query() Error\n");
		return NULL;
	}

	_LogFileWrite(_LOGPATH,1,"GetTableElement Start\n");
	_CallDebug(1,"GetTableElement Start...\n");
	return mysql_store_result(mysql);
}

inline void GetTableElementEnd(MYSQL_RES* res)
{
	mysql_free_result(res);
	_LogFileWrite(_LOGPATH,1,"GetTableElement End\n");
	_CallDebug(1,"GetTableElement End\n");
}
