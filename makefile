CP :=g++
AP1 :=-Wall
AP2 :=-Wall
STD :=-std=c++11
DEBUG :=-D _DEBUG -g
# 头文件
CPPFLAGS := -I/usr/include/mysql \
			-I/usr/local/include/mysql++
# 库目录
LDFLAGS := -L/usr/local/lib \
			-L/usr/lib64/mysq
# 动态库文件
LDLIBS := -lmysqlpp \
		-lmysqlclient 
CPPMYSQL :=$(CPPFLAGS) $(LDFAGS) -lmysqlpp 

all:

config: ./_config/_config.* 
	$(CP) $(AP1) $(STD) ./_config/_config.cc

mylibs: ./_config/_config.* ./mylibs.*
	$(CP) $(AP1) $(STD) ./_config/_config.cc ./mylibs.cc 

tcp: ./_config/_config.* ./mylibs.* ./_tcp/tcp.* 
	$(CP) $(AP1) $(STD) ./_config/_config.cc ./mylibs.cc \
	./_tcp/tcp.cc 

database: ./_config/_config.* ./mylibs.* ./_database/db.*
	$(CP) $(AP1) $(STD) $(CPPMYSQL) ./_config/_config.cc \
	./mylibs.cc ./_database/db.cc

response: ./_config/_config.* ./mylibs.* ./_tcp/tcp.* \
		./_database/db.* ./_http/_response.*
	$(CP) $(AP1) $(STD) $(CPPMYSQL) ./_config/_config.cc \
	./mylibs.cc ./_tcp/tcp.cc ./_database/db.cc \
	./_http/_response.cc

http: ./_config/_config.* ./mylibs.* ./_tcp/tcp.* \
	./_database/db.* ./_http/_response.* ./_http/http.*  
	$(CP) $(AP1) $(STD) $(CPPMYSQL) ./_config/_config.cc \
	./mylibs.cc ./_tcp/tcp.cc ./_database/db.cc \
	./_http/_response.cc ./_http/http.cc

httpserver: ./_config/_config.* ./mylibs.* ./_tcp/tcp.* \
			./_http/_response.* ./_http/http.* ./_database/db.* \
			./_http/http.* ./httpserver.cc
	$(CP) $(AP1) $(STD) $(DEBUG) $(CPPMYSQL) ./_config/_config.cc \
	./mylibs.cc ./_tcp/tcp.cc ./_database/db.cc ./_http/_response.cc \
	./_http/http.cc ./httpserver.cc -o httpserver
#	export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

openport:
	firewall-cmd --zone=public --add-port=80/tcp --permanent

httpbuild:
	make clean && make httpserver

httptest:
	make openport && make clean && make httpserver && ./httpserver

clean:
	rm -rf ./_config/_config.h.gch ./mylibs.h.gch ./_tcp/tcp.h.gch \
	./_http/http.h.gch ./_http/_response.h.gch ./_database/db.h.gch \
	./httpserver ./httpserver_log.log
