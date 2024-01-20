#include"mylibs.h"

/**
 * 构造函数
 * 用于构建整个后端项目中唯一的日志器
**/
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

/**
 * 类预加载的当前类实例获取函数
 * 用于构建整个后端项目中唯一的MyLibs对象实例
**/

MyLibs& MyLibs::GetMyself() 
{
	static MyLibs mylibs;
	return mylibs;
}

/**
 * Debug调试信息输出函数
 * 利用函数重载的方式实现变动参数的添加
**/
/*********************************************************/
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
/*********************************************************/

/**
 * 日志文件的写入函数
 * 主要有Info(通知类型)与Error(错误类型)
**/
/*********************************************************/
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
/*********************************************************/

// 整数求n的m次方
int MyLibs::_pow(int n,int m)
{
	if(0 == m){ return 1; }
	while(--m){ n *= n; }
	return n;
} 

/**
 * char []，字符数组转换为整型数据
 * 删除两端空白，从右向左遍历，每进一位，权位进一
 * 就是每进一位，当前进位值比之前的值大10倍
**/ 
int MyLibs::_stoi(const char* strs)
{
	int left = 0,right = strlen(strs) - 1;
	int ans = 0,count = 0;
	while(isspace(strs[left])){ ++left; }
	while(isspace(strs[right])){ --right; }

	while(right >= left && isdigit(strs[right]))
	{
		int temp = (strs[right] - 48);
		ans = (temp * _pow(10,count++)) + ans;
		--right;
	}
	return ans;
}

/**
 * 前缀匹配函数
 * 相当于简单的字符比较，buf与prefix字符串进行匹配，之后再返回prefix的长度
**/
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

