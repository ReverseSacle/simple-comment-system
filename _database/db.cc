#include"db.h"

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

bool DataBase::TableIsNull()
{
	mysqlpp::Query query = conn.query();
	std::string _sql;

	_sql += (
		"SELECT COUNT(*) FROM information_schema.tables"
		" WHERE table_schema = '" + db_name + 
		"' AND table_name = '" + table_name + "';"
	);
	MyLibs::CallDebug(
		"DataBase::TableIsNull() => query\n",
		_sql
	);
	query << _sql;	

	if(0 == static_cast<int>(query.store()[0][0])){
		return false;
	} 	
	return true;
}

int DataBase::TableRecordCount()
{
	if(false == TableIsNull()){
		return 0;
	}

	mysqlpp::Query query = conn.query();
	std::string _sql;

	_sql += (
		"SELECT COUNT(*) FROM " + table_name +
		";"
	);
	MyLibs::CallDebug(
		"DataBase::TableRecordCount() => query\n",
		_sql
	);
	query << _sql;	

	return static_cast<int>(query.store()[0][0]);
}

int DataBase::GetTableParentRecordNum()
{
	if(false == TableIsNull()){
		return 0;
	}

	mysqlpp::Query query = conn.query();
	std::string _sql;

	_sql += (
		"SELECT COUNT(*) FROM " + table_name +
		" WHERE parent_id IS NULL" 
		" AND response_id IS NULL" 
		";"
	);
	MyLibs::CallDebug(
		"DataBase::GetTableParentRecordNum() => query\n",
		_sql
	);
	query << _sql;	

	return static_cast<int>(query.store()[0][0]);
}

bool DataBase::TableInsert(Record* record)
{
	try{
		std::string _sql;
		mysqlpp::Query query = conn.query();
		int count = 0;

		if(false == this->TableIsNull())
		{
			mysqlpp::Query query_create = conn.query();

			MyLibs::CallDebug(
				"DataBase::TableInsert => Table is NULL"
			);
			_sql += "CREATE TABLE ";
			_sql += table_name;
			_sql += "(";
			_sql += "\nid CHAR(8) PRIMARY KEY,";
			_sql += "\nparent_id CHAR(8),";
			_sql += "\nresponse_id CHAR(8),";
			_sql += "\nnick_name CHAR(64),";
			_sql += "\nmail CHAR(64),";
			_sql += "\nmail_md5 CHAR(65),";
			_sql += "\ncreate_at CHAR(16),";
			_sql += "\ncomment TEXT";
			_sql += "\n);";

			MyLibs::CallLogInfo(
				"DataBase::TableInsert => query\n",
				_sql
			);
			MyLibs::CallDebug(
				"DataBase::TableInsert => query\n",
				_sql
			);

			query_create << _sql;
			query_create.execute();
			_sql.clear();			
		} else { count = TableRecordCount(); }

		_sql += "INSERT INTO ";
		_sql += table_name;
		_sql += "(id,parent_id,response_id,nick_name,mail,mail_md5,create_at,comment)\n";
		_sql += "VALUES('";
		_sql += std::to_string(count + 1) + "',";

		if((record->parent_id).empty() && (record->response_id).empty())
		{
			_sql += "NULL,";
			_sql += "NULL,'";
		}
		else
		{
			_sql += "'";
			_sql += record->parent_id + "','";
			_sql += record->response_id + "','";
		}
		_sql += record->nick_name + "','";
		_sql += record->mail + "','";
		_sql += record->mail_md5 + "','";
		_sql += record->create_at +"','";
		_sql += record->comment;
		_sql += "');"; 

		// log...
		MyLibs::CallLogInfo(
			"DataBase::TableInsert() => query\n" 
			"INSERT INTO " + table_name + 
			"(id,parent_id,response_id,nick_name,mail,mail_md5,create_at,comment)\n"	
			"VALUES('",
			std::to_string(count + 1) + "','" + 
			record->parent_id + "','" + 
			record->response_id + "','" + 
			record->nick_name + "','" + 
			record->mail + "','" +
			record->mail_md5 + "','" +
			record->create_at + "','" + 
			record->comment + 
			"');"
		);
		MyLibs::CallDebug(
			"DataBase::TableInsert() => query\n" 
			"INSERT INTO " + table_name + 
			"(id,parent_id,response_id,nick_name,mail,mail_md5,create_at,comment)\n"	
			"VALUES('",
			std::to_string(count + 1) + "','" + 
			record->parent_id + "','" + 
			record->response_id + "','" + 
			record->nick_name + "','" + 
			record->mail + "','" +
			record->mail_md5 + "','" +
			record->create_at + "','" + 
			record->comment + 
			"');"
		);

		query << _sql;
		query.execute();
	} catch(const mysqlpp::Exception& er) {
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

bool DataBase::GetTableParentRecord(std::vector<Record*>& buf)
{
	mysqlpp::Query query = conn.query();

	if(false == TableIsNull()){
		return false;
	}
	std::string _sql;

	_sql += "SELECT * FROM ";
	_sql += table_name;
	_sql += " WHERE parent_id IS NULL";
	_sql += " AND response_id IS NULL";
	_sql += " ORDER BY create_at";
	_sql += ";";

	// log...
	MyLibs::CallLogInfo(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM " + table_name,
		" WHERE parent_id IS NULL" 
		" AND response_id IS NULL" 
		" ORDER BY create_at"	
		";"
	);
	MyLibs::CallDebug(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM " + table_name,
		" WHERE parent_id IS NULL" 
		" AND response_id IS NULL" 
		" ORDER BY create_at"	
		";"
	);

	query << _sql;
	if(mysqlpp::StoreQueryResult res = query.store())
	{
		for(auto& row: res)
		{
			Record* record = new Record();
			record->id = std::string(row["id"]);
			record->parent_id = std::string(row["parent_id"]);
			record->response_id = std::string(row["response_id"]);
			record->nick_name = std::string(row["nick_name"]);
			record->mail = std::string(row["mail"]);
			record->mail_md5 = std::string(row["mail_md5"]);
			record->create_at = std::string(row["create_at"]);
			record->comment = std::string(row["comment"]);
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

bool DataBase::GetTableChildRecord(std::vector<Record*>& buf)
{
	mysqlpp::Query query = conn.query();

	if(false == TableIsNull()){
		return false;
	}
	std::string _sql;

	_sql += "SELECT * FROM ";
	_sql += table_name;
	_sql += " WHERE parent_id IS NOT NULL";
	_sql += " AND response_id IS NOT NULL";
	_sql += " ORDER BY create_at";
	_sql += ";";

	// log...
	MyLibs::CallLogInfo(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM " + table_name,
		" WHERE parent_id IS NOT NULL" 
		" AND response_id IS NOT NULL" 
		" ORDER BY create_at"
		";"
	);
	MyLibs::CallDebug(
		"DataBase::GetTableRecord() => query\n"
		"SELECT * FROM " + table_name,
		" WHERE parent_id IS NOT NULL" 
		" AND response_id IS NOT NULL" 
		" ORDER BY create_at"
		";"
	);

	query << _sql;
	if(mysqlpp::StoreQueryResult res = query.store())
	{
		for(auto& row: res)
		{
			Record* record = new Record();
			record->id = std::string(row["id"]);
			record->parent_id = std::string(row["parent_id"]);
			record->response_id = std::string(row["response_id"]);
			record->nick_name = std::string(row["nick_name"]);
			record->mail = std::string(row["mail"]);
			record->mail_md5 = std::string(row["mail_md5"]);
			record->create_at = std::string(row["create_at"]);
			record->comment = std::string(row["comment"]);
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
