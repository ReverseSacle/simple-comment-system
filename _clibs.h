#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<errno.h>
#include<signal.h>
#include<stdarg.h>

#define i16 short
#define i32 int
#define i64 long
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long
#define _ROOTDIR "docs"
#define _LOGPATH "./_httpserver.log"
#define _DEBUG

i32 _pow(i32 n,i32 m);
i32 _stoi(const char* strs);
inline void _CallDebug(const i32 num,...);
void _LogFileWrite(const char* source,const i32 num,...);