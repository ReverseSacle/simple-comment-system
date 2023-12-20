#pragma once
#include<iostream>
#include<ctype.h>
#include<errno.h>
#include<string.h>
#include<string>
#include<vector>

class MyLibs
{
public:
	static void CallDebug(const std::string& str1);
	static void CallDebug(const std::string& str1,const std::string& str2);
	static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3);
	static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4);
	static void CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4,const std::string& str5);
	// 前缀匹配
	static bool StartWith(const std::string& buf,const std::string& prefix,size_t& index);
	static int _pow(int n,int m);// 平方
	static int _stoi(const char* strs);// string to int 
};
