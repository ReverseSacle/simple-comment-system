OP=gcc
AP1=-Wall
AP2=-g
LOG=-D _LOG
DEBUG=-D _DEBUG

MYSQL=$$(mysql_config --cflags --libs)
MSPATH=-L/usr/lib64/mysql -lmysqlclient 

tcp: ./_tcp/tcp.*
	$(OP) $(AP1) ./_tcp/tcp.c 

http_response: ./_tcp/tcp.* ./_http/_response.* ./mylibs.*
	$(OP) $(AP1) ./_tcp/tcp.c ./_http/_response.c

http_response_db: ./_tcp/tcp.* ./_http/_response.* ./mylibs.* ./_database/db.*
	$(OP) $(AP1) ./_tcp/tcp.c ./_http/_response.c ./_database/db.c

http: ./_tcp/tcp.* ./_http/http.* ./mylibs.*
	$(OP) $(AP1) ./_tcp/tcp.c ./_http/http.c ./mylibs.c

database: ./_database/db.* ./mylibs.*
	$(OP) $(AP1) ./_database/db.c ./mylibs.c 

httptest: ./_tcp/tcp.* ./_http/http.* ./_http/_response.* ./mylibs.* ./httpserver.c
	$(OP) $(AP1) $(DEBUG) ./_tcp/tcp.c ./_http/http.c ./_http/_response.c \
	./mylibs.c httpserver.c -o httpserver 

httptest_db: ./_tcp/tcp.* ./_http/http.* ./_http/_response.* \
			./mylibs.* ./httpserver.c ./_database/db.*
	$(OP) $(AP1) $(DEBUG) $(MSPATH) ./_tcp/tcp.c ./_http/http.c ./_http/_response.c \
	./mylibs.c ./_database/db.c httpserver.c -o httpserver 

httptest_db_log: ./_tcp/tcp.* ./_http/http.* ./_http/_response.* \
			./mylibs.* ./httpserver.c ./_database/db.*
	$(OP) $(AP1) $(DEBUG) $(LOG) $(MSPATH) ./_tcp/tcp.c ./_http/http.c ./_http/_response.c \
	./mylibs.c ./_database/db.c httpserver.c -o httpserver 

openport:
	firewall-cmd --zone=public --add-port=80/tcp --permanent

clean:
	rm -rf httpserver *.a *.log *.so
