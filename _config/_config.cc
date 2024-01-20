#include"_config.h"

/**
 * 构造函数
 * 加载`_config.json`文件中的配置信息，通过第三方json库来解析
**/
MyConfig::MyConfig()
{
	std::ifstream json_file(_CONFIGFILEPATH);
	if(!json_file.is_open()){ return; }

	using json = nlohmann::json;
	json json_data = json::parse(json_file);

	if(!json_data["root_dir"].is_null()){
		root_dir = json_data["root_dir"];
	}

	if(!json_data["log"].is_null())
	{
		auto& log_config = json_data["log"];
		if(!log_config["enable"].is_null())
		{
			LogConfig.enable = log_config["enable"];
			if(LogConfig.enable && !log_config["path"].is_null()){
				LogConfig.path = json_data["log"]["path"];
			} else {
				LogConfig.enable = false;
			}
		}
	}

	if(!json_data["database"].is_null())
	{
		auto& db_config = json_data["database"];

		if(!(
			db_config["host"].is_null() || 
			db_config["user"].is_null() || 
			db_config["password"].is_null() ||
			db_config["database_name"].is_null() ||
			db_config["table_name"].is_null()
		)){ 
			DatabaseConfig.host = db_config["host"];
			DatabaseConfig.user = db_config["user"];
			DatabaseConfig.password = db_config["password"];
			DatabaseConfig.database_name = db_config["database_name"];
			DatabaseConfig.table_name = db_config["table_name"];
		}
	}
}

/**
 * 获取MyConfig实例对象函数
 * 通过static关键字的特性创建整个项目唯一的MyConfig实例对象
**/

MyConfig& MyConfig::GetMyself()
{
	static MyConfig config;
	return config;
}

std::string MyConfig::GetRootDir(){
	return GetMyself().root_dir;
}

decltype(MyConfig::DatabaseConfig)& MyConfig::GetDatabaseConfig(){
	return GetMyself().DatabaseConfig;
}

decltype(MyConfig::LogConfig)& MyConfig::GetLogConfig(){
	return GetMyself().LogConfig;
}
