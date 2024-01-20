#pragma once
#include<fstream>
#include"json.hpp"

#define _CONFIGFILEPATH "./_config/_config.json"

class MyConfig
{
private:
	// 数据库相关的配置信息
	struct
	{
		std::string host;
		std::string user;
		std::string password;
		std::string database_name;
		std::string table_name;
	}DatabaseConfig;
	// 日志器相关的配置信息
	struct
	{
		bool enable = false;
		std::string path = "./httpserver_log.log";
	}LogConfig;
	std::string root_dir = "docs";// 服务器的文件的根目录
	MyConfig();
public:
	static MyConfig& GetMyself();
	static std::string GetRootDir();
	static decltype(DatabaseConfig)& GetDatabaseConfig();
	static decltype(LogConfig)& GetLogConfig();
};

