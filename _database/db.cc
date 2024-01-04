#include"db.h"

bool DataBase::table_isnull(mysqlpp::Connection& connector,std::string& db_name,std::string& table_name)
{
	mysqlpp::Query query = connector.query();
	std::string _sql;
	
	_sql += ("SELECT COUNT(*) FROM information_schema.tables"
			" WHERE table_schema = '" + db_name + 
			"' AND table_name = '" + table_name + "';"
	);
	MyLibs::CallDebug(
		"DataBase::table_isnull() => query\n",
		_sql
	);
	query << _sql;	
	
	mysqlpp::StoreQueryResult res = query.store();
	if(0 == static_cast<int>(res[0][0])){
		MyLibs::CallDebug(
			"DataBase::table_isnull() => true"
		);
		return true;
	}
	MyLibs::CallDebug(
		"DataBase::table_isnull() => false"
	);
	return false;
}

bool DataBase::Connect()
{
	auto& db_config = MyConfig::GetDatabaseConfig();
	bool db_connect = conn.connect(
		db_config.database_name.c_str(),
		db_config.host.c_str(),
		db_config.user.c_str(),
		db_config.password.c_str()
	);

	db_name = db_config.database_name;
	table_name = db_config.table_name;
	if(false == db_connect)
	{
		// log...
		MyLibs::CallLogInfo(
			"DataBase::Connect() => conn.connect() fail"
		);
		MyLibs::CallDebug(
			"DataBase::Connect() => conn.connect() fail"
		);
		return false;
	}
	// log...
	MyLibs::CallLogInfo(
		"DataBase::Connect() => Succeed"
	);
	MyLibs::CallDebug(
		"DataBase::Connect() => Succeed"
	);
	return true;
}

bool DataBase::TableInsert(Record* record)
{
	try{
		mysqlpp::Query query = conn.query();
		std::string _sql;

		if(table_isnull(conn,db_name,table_name))
		{
			mysqlpp::Query query_create = conn.query();
			_sql += "CREATE TABLE ";
			_sql += table_name;
			_sql += "(";
			_sql += "\nNickName CHAR(64),";
			_sql += "\nEmail CHAR(64),";
			_sql += "\nEmailMd5 CHAR(65),";
			_sql += "\nCreateAt CHAR(16),";
			_sql += "\nContent TEXT";
			_sql += "\n);";

			MyLibs::CallDebug(
				"DataBase::Connect() =>	query\n",
				"CREATE TABLE " + table_name,
				"("
				"\nNickName CHAR(64),"
				"\nEmail CHAR(64),"
				"\nEmailMd5 CHAR(65),"
				"\nCreateAt CHAR(16),"
				"\nContent TEXT"	
				"\n);"	
			);
			query_create << _sql;
			query_create.execute();
			_sql.clear();			
		}

		_sql += "INSERT INTO ";
		_sql += table_name;
		_sql += "(NickName,Email,EmailMd5,CreateAt,Content)\n";
		_sql += "VALUES('";
		_sql += record->nickname + "','";
		_sql += record->email + "','";
		_sql += record->email_md5 + "','";
		_sql += record->create_at +"','";
		_sql += record->content;
		_sql += "');"; 

		// log...
		MyLibs::CallLogInfo(
			"DataBase::TableInsert() => query\n" 
			"INSERT INTO " + table_name + 
			"(NickName,Email,EmailMd5,CreateAt,Content)\n"
			"VALUES('",
			record->nickname + "','" + 
			record->email + "','" +
			record->email_md5 + "','" +
			record->create_at + "','" + 
			record->content + 
			"');"
		);
		MyLibs::CallDebug(
			"DataBase::TableInsert() => query\n" 
			"INSERT INTO " + table_name + 
			"(NickName,Email,EmailMd5,CreateAt,Content)\n"
			"VALUES('" + 
			record->nickname + "','" + 
			record->email + "','" + 
			record->email_md5 + "','" +
			record->create_at + "','" + 
			record->content + 
			"');"
		);

		query << _sql;
		query.execute();
	} catch(const mysqlpp::Exception& er) {
		// log...
		MyLibs::CallLogError(
			"DataBase::TableInsert() => query fail.\nreason: ",
			er.what()
		);
		MyLibs::CallDebug(
			"DataBase::TableInsert() => query fail.\nreason: ",
			er.what()
		);
		return false;
	}

	// log...
	MyLibs::CallLogInfo(
		"DataBase::TableInsert() => Succeed"
	);
	MyLibs::CallDebug(
		"DataBase::TableInsert() => Succeed"
	);
	return true;
}

bool DataBase::GetTableRecord(std::vector<Record*>& buf)
{
	mysqlpp::Query query = conn.query();

	if(table_isnull(conn,db_name,table_name)){
		return false;
	}
	std::string _sql;

	_sql += "SELECT * FROM ";
	_sql += table_name;
	_sql += ";";

	// log...
	MyLibs::CallLogInfo(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM ", 
		table_name,
		";"
	);
	MyLibs::CallDebug(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM ", 
		table_name,
		";"
	);

	query << _sql;
	if(mysqlpp::StoreQueryResult res = query.store())
	{
		for(auto& row: res)
		{
			Record* record = new Record();
			record->nickname = std::string(row["NickName"]);
			record->email = std::string(row["Email"]);
			record->email_md5 = std::string(row["EmailMd5"]);
			record->create_at = std::string(row["CreateAt"]);
			record->content = std::string(row["Content"]);
			buf.push_back(record);
		}
	}
	else
	{
		// log...
		MyLibs::CallLogError(
			"DataBase::GetTableRecord() => query.store() fail.\nreason: ",
			query.error()
		);
		MyLibs::CallDebug(
			"DataBase::GetTableRecord() => query.store() fail.\nreason: ",
			query.error()
		);
		return false;
	}

	// log...
	MyLibs::CallLogInfo(
		"DataBase::GetTableRecord() => Succeed"
	);
	MyLibs::CallDebug(
		"DataBase::GetTableRecord() => Succeed"
	);
	return true;
}	
