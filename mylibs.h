#pragma once
#include<ctype.h>
#include<string.h>
#include<errno.h>

#include<iostream>
#include<string>
#include<vector>

#include<spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#define _LOGFILENAME "httpserver_log.log"

extern bool log_option;

class MyLibs
{
private:
	MyLibs();
	std::shared_ptr<spdlog::logger> file_logger;
public:
    static MyLibs& GetLogger();// 获取logger
	// CallDebug..............
    static void CallDebug(const std::string& str1) noexcept;
    static void CallDebug(const std::string& str1,const std::string& str2) noexcept;
    static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3) noexcept;
    static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4) noexcept;
	/****************************/
	// CallLogInfo...............
	static void CallLogInfo(const std::string& str1);
	static void CallLogInfo(const std::string& str1,const std::string& str2);
	static void CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3);
	static void CallLogInfo(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4);
	// CallLogError.............
	static void CallLogError(const std::string& str1);
	static void CallLogError(const std::string& str1,const std::string& str2);
	static void CallLogError(const std::string& str1,const std::string& str2,const std::string& str3);
	static void CallLogError(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4);
	/**********************/
	static int _pow(int n,int m);// 求平方
	static int _stoi(const char* strs);// char [] to int 
	// 前缀匹配
	static bool StartWith(const std::string& buf,const std::string& prefix,size_t& index) noexcept;
};
