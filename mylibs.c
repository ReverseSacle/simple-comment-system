#include"mylibs.h"

int _pow(int n,int m)
{
	if(0 == m){ return 1; }
	while(--m){ n *= n; }
	return n;
} 

int _stoi(const char* strs)
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

void _CallDebug(const size_t num,...)
{
	#ifdef _DEBUG
	{
		size_t i = 0;
		char buf[1024] = {'\0'};
		va_list args;

		va_start(args,num);
		while(i < num)
		{
			char* strs = va_arg(args,char*); 
			strcat(buf,strs);
			++i;
		} 
		va_end(args);

		fprintf(stdout,buf);
	}
	#endif
}

void _LogFileWrite(const char* filename,const size_t num,...)
{
	#ifdef _LOG
	{
		FILE* source = fopen(filename,"a+");
		if(NULL == source){ return; }

		va_list args;
		size_t i = 0;

		fputs(__TIME__,source);
		fputs(":\n",source);
		va_start(args,num);
		while(i < num)
		{
			char* strs = va_arg(args,char*);
			fputs(strs,source);
			++i;
		}
		va_end(args);
		fclose(source);
	}
	#endif
}
