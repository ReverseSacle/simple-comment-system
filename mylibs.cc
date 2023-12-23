#include"mylibs.h"

MyLibs::MyLibs()
{
	file_logger = spdlog::basic_logger_mt("my_logger", _LOGFILENAME);
	file_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

MyLibs& MyLibs::GetLogger() 
{
	static MyLibs logger;
	return logger;
}

void MyLibs::CallDebug(const std::string& str1) noexcept
{
    #ifdef _DEBUG
    std::cout << str1 << std::endl;
    #endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2) noexcept
{
    #ifdef _DEBUG
    std::cout << str1 << str2 << std::endl;
    #endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2,const std::string& str3) noexcept
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << str3 << std::endl;
	#endif
}

void MyLibs::CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4) noexcept
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << str3 << str4 << std::endl;
	#endif
}

void MyLibs::CallLogInfo(const std::string& str1)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::info);
	GetLogger().file_logger->info("\n" + str1);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::info);
	GetLogger().file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::info);
	GetLogger().file_logger->info("\n" + str1 + str2 + str3);
}

void MyLibs::CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::info);
	GetLogger().file_logger->info("\n" + str1 + str2 + str3 + str4);
}

void MyLibs::CallLogError(const std::string& str1)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::err);
	GetLogger().file_logger->info("\n" + str1);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::err);
	GetLogger().file_logger->info("\n" + str1 + str2);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2,const std::string& str3)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::err);
	GetLogger().file_logger->info("\n" + str1 + str2 + str3);
}

void MyLibs::CallLogError(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4)
{
	if(false == log_option){ return; }
	GetLogger().file_logger->set_level(spdlog::level::err);
	GetLogger().file_logger->info("\n" + str1 + str2 + str3 + str4);
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

bool MyLibs::StartWith(const std::string& buf,const std::string& prefix,size_t& index) noexcept
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

