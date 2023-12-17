#pragma once
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdarg.h>

#define _LOGPATH "./_httpserver.log"

typedef unsigned int usize_t;

int _pow(int n,int m);
int _stoi(const char* strs);
void _CallDebug(const size_t num,...);
void _LogFileWrite(const char* filename,const size_t num,...);
