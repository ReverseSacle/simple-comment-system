#include"mylibs.h"

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

void MyLibs::CallDebug(const std::string& str1,const std::string& str2,const std::string& str3,const std::string& str4,const std::string& str5)
{
	#ifdef _DEBUG
	std::cout << str1 << str2 << str3 << str4 << str5 << std::endl;
	#endif
}

bool MyLibs::StartWith(const std::string& buf,const std::string& prefix,size_t& index)
{
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
