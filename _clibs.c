#include"_clibs.h"

i32 _pow(i32 n,i32 m)
{
	if(0 == m) return 1;
	while(--m){ n *= n; }
	return n;
} 

i32 _stoi(const char* strs)
{
	i32 left = 0,right = 0;
	i32 ans = 0,count = 0;
	while(isspace(strs[left])) ++left;

	right = strlen(strs) - 1;
	while(right >= left)
	{
		i32 t = (strs[right] - 48);
		ans = (t * _pow(10,count++)) + ans;
		--right;
	}
	return ans;
}

inline void _CallDebug(const i32 num,...)
{
	#ifdef _DEBUG
	{
		i32 i = 0;
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

void _LogFileWrite(const char* filename,const i32 num,...)
{
	pthread_mutex_lock(&mutex1);
	FILE* source = fopen(filename,"a+");
	if(NULL == source){ return; }

	va_list args;
	i32 i = 0;

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
	pthread_mutex_unlock(&mutex1);
}
