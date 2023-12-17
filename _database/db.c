#include"db.h"

MYSQL* DatabaseConnect()
{
	MYSQL* mysql = mysql_init(NULL);
	if(NULL == mysql)
	{
		_LogFileWrite(_LOGPATH,1,"DatabaseConnect() => mysql_init() fail\n");
		_CallDebug(1,"Database_DatabaseConnect() => mysql_init() fail\n");
		return NULL;
	}

	mysql = mysql_real_connect(mysql,_HOST,_USER,_PASSWORD,_DBNAME,0,NULL,0);
	if(NULL == mysql)
	{
		_LogFileWrite(_LOGPATH,1,"DatabaseConnect() => mysql_real_connect() fail\n");
		_CallDebug(1,"Database_DatabaseConnect() => mysql_real_connect() fail\n");
		return NULL;
	}

	_LogFileWrite(_LOGPATH,1,"Database_DatabaseConnect() => Succeed\n");
	_CallDebug(1,"Database_DatabaseConnect() => Succeed\n");
	return mysql;
}

inline void DatabaseConnectClose(MYSQL* mysql)
{
	mysql_close(mysql);
	_LogFileWrite(_LOGPATH,1,"Database_DatabaseConnect() => Close\n");
	_CallDebug(1,"Database_DatabaseConnect() => Close\n");
}

void TableInsert(MYSQL* mysql,const struct Record* el)
{
	char buf[1024] = {'\0'};

	sprintf(
		buf,
		"INSERT INTO %s(NickName,Email,Content,CreateAt)\nVALUES('%s','%s','%s','%s')",el->tablename,el->nickname,el->email,el->content,el->createat
	);

	_LogFileWrite(_LOGPATH,3,"Database_TableInsert() => \nQuery:\n",buf,"\n");
	_CallDebug(3,"Database_TableInsert() => \nQuery:\n",buf,"\n");
	if(0 != mysql_query(mysql,buf))
	{
		_LogFileWrite(
			_LOGPATH,3,"Database_TableInsert() => mysql_query() fail.\nreason: \n",
			mysql_error(mysql),"\n"
		);
		_CallDebug(
			3,"Database_TableInsert() => mysql_query() fail.\nreason: \n",
			mysql_error(mysql),"\n"
		);
		return;
	}
	_LogFileWrite(_LOGPATH,1,"Database_TableInsert() => Succeed\n");
	_CallDebug(1,"Database_TableInsert() => Succeed\n");		
}

MYSQL_RES* GetTableRecordStart(MYSQL* mysql,const char* tableName)
{
	char temp[64] = {'\0'};

	sprintf(temp,"SELECT * FROM %s ORDER BY CreateAT ASC",tableName);
	if(0 != mysql_query(mysql,temp))
	{
		_LogFileWrite(
			_LOGPATH,3,"Database_GetTableRecordStart() => mysql_query() fail\n.reason:\n",
			mysql_error(mysql),"\n"
		);
		_CallDebug(
			3,"Database_GetTableRecordStart() => mysql_query() fail\n.reason:\n",
			mysql_error(mysql),"\n"
		);
		return NULL;
	}

	_LogFileWrite(_LOGPATH,1,"Database_GetTableRecordStart() => Succeed\n");
	_CallDebug(1,"Database_GetTableRecordStart() => Succeed\n");
	return mysql_store_result(mysql);
}

inline void GetTableRecordEnd(MYSQL_RES* res)
{
	mysql_free_result(res);
	_LogFileWrite(_LOGPATH,1,"Database_GetTableRecordEnd()\n");
	_CallDebug(1,"Database_GetTableRecordEnd()\n");
}
