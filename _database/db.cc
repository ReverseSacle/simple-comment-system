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
		std::string _sql = "INSERT INTO ";

		_sql += table_name;
		_sql += "(NickName,Email,Content,CreateAt)\nVALUES(";
		_sql +=	mysqlpp::quote + "'";
		_sql += record->nickname + "','";
		_sql +=	mysqlpp::quote + "";
		_sql += record->email + "','";
		_sql +=	mysqlpp::quote + "";
		_sql += record->content + "','";
		_sql +=	mysqlpp::quote + "";
		_sql += record->createat +"'";
		_sql += ");"; 

		// log...
		MyLibs::CallLogInfo(
			"DataBase::TableInsert() => query\n", 
			"INSERT INTO " + table_name + 
			"(NickName,Email,Content,CreateAt)\nVALUES('",
			record->nickname + "','" + record->email + "','",
			record->content + "','" + record->createat + "');"
		);
		MyLibs::CallDebug(
			"DataBase::TableInsert() => query\n", 
			"INSERT INTO " + table_name + 
			"(NickName,Email,Content,CreateAt)\nVALUES('" + 
			record->nickname + "','" + record->email + "','" + 
			record->content + "','" + record->createat + "');"
		);

		query << _sql << std::endl;
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

bool DataBase::GetTableRecord(std::vector<Record>& buf)
{
	mysqlpp::Query query = conn.query();
	std::string _sql = "SELECT * FROM " + mysqlpp::quote;

	_sql += table_name;
	_sql += ";";

	// log...
	MyLibs::CallLogInfo(
		"DataBase::TableInsert() => query\nSELECT * FROM ", 
		table_name,
		";"
	);
	MyLibs::CallDebug(
		"DataBase::TableInsert() => query\nSELECT * FROM ", 
		table_name,
		";"
	);

	query << _sql << std::endl;
	if(mysqlpp::StoreQueryResult res = query.store())
	{
		for(auto& row: res)
		{
			Record record;
			record.nickname = static_cast<std::string>(row[0]);
			record.email = static_cast<std::string>(row[1]);
			record.content = static_cast<std::string>(row[2]);
			record.createat = static_cast<std::string>(row[3]);
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
