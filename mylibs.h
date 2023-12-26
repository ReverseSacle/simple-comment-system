#pragma once
#include<ctype.h>
#include<string.h>
#include<errno.h>

#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include<spdlog/spdlog.h>
#include<spdlog/sinks/basic_file_sink.h>
#include"./_config/json.hpp"

#define _LOGFILEPATH "./httpserver_log.log"
#define _CONFIGFILEPATH "./_config/_config.json"

class MyLibs
{
private:
	MyLibs();
	std::shared_ptr<spdlog::logger> file_logger;
	bool log_option;
	std::string root_dir;
public:
    static MyLibs& GetMyself();// 获取唯一MyLibs对象
	static std::string GetRootDir();
	// CallDebug..............
    static void CallDebug(const std::string& str1);
    static void CallDebug(const std::string& str1,const std::string& str2) ;
    static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3) ;
    static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4) ;
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
	static bool StartWith(const std::string& buf,const std::string& prefix,size_t& index) ;
};
