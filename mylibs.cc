#include"mylibs.h"

MyLibs::MyLibs()
{
	if(MyConfig::GetLogConfig().enable)
	{
		file_logger = spdlog::basic_logger_mt(
			"my_logger",
			MyConfig::GetLogConfig().path
		);
		file_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
	}
}

MyLibs& MyLibs::GetMyself() 
{
	static MyLibs mylibs;
	return mylibs;
}

void MyLibs::CallDebug(const std::string& str1) 
{
	#ifdef _DEBUG
	std::cout << str1 << std::endl;
	#endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2) 
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << std::endl;
	#endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2,const std::string& str3) 
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << str3 << std::endl;
	#endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4) 
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << str3 << str4 << std::endl;
	#endif
}

void MyLibs::CallLogInfo(const std::string& str1)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
}

void MyLibs::CallLogError(const std::string& str1)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2,const std::string& str3)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4)
{
	auto& _this = GetMyself();
	if(false == MyConfig::GetLogConfig().enable){ return; }

	_this.file_logger->set_level(spdlog::level::info);
	_this.file_logger->info("\n" + str1 + str2);
}

int MyLibs::_pow(int n,int m)
{
	if(0 == m){ return 1; }
	while(--m){ n *= n; }
	return n;
} 

int MyLibs::_stoi(const char* strs)
{
	int left = 0,right = 0;
	int ans = 0,count = 0;
	while(isspace(strs[left])){ ++left; }

	right = strlen(strs) - 1;
	while(right >= left)
	{
		int temp = (strs[right] - 48);
		ans = (temp * _pow(10,count++)) + ans;
		--right;
	}
	return ans;
}

bool MyLibs::StartWith(const std::string& buf,const std::string& prefix,size_t& index) 
{
	if(buf.empty() || prefix.empty())
	{ 
		index = 0;
		return false; 
	}
	size_t i = 0;

	while(i < prefix.size())
	{
		if(buf[i] != prefix[i]){
			return false;
		}
		++i;
	}
	index = i;	

	return true;
}

