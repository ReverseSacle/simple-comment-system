OP=gcc
AP1=-Wall
AP2=-g

MYSQL=$$(mysql_config --cflags --libs)
MSPATH=-L/usr/lib64/mysql -lmysqlclient 
LIBPATH=-L. -l_slibs

DM=-fPIC -shared

all: httpserver

#Product
httpserver: httpserver.c http.h http.c database.h database.c _clibs.h _clibs.c
	$(OP) -c _clibs.c
	$(OP) -c database.c 
	$(OP) -c http.c
	ar rcs lib_slibs.a _clibs.o database.o http.o
	rm -rf _clibs.o database.o http.o
	$(OP) $(AP1) -O2 $(AP2) $(MSPATH) httpserver.c lib_slibs.a -o httpserver 

#Static
#httpserver: httpserver.c http.h http.c database.h database.c _clibs.h _clibs.c
#	$(OP) -c _clibs.c
#	$(OP) -c database.c 
#	$(OP) -c http.c
#	ar rcs lib_slibs.a _clibs.o database.o http.o
#	rm -rf _clibs.o database.o http.o
#	$(OP) $(AP1) $(AP2) $(MSPATH) -pthread httpserver.c lib_slibs.a -o httpserver 

#Dynamic
#httpserver: httpserver.c http.h http.c database.h database.c _clibs.h _clibs.c
#	$(OP) $(DM) $(MYSQL) http.c database.c _clibs.c -o lib_slibs.so
#	$(OP) $(AP1) $(AP2) $(LIBPATH) -pthread httpserver.c -o httpserver 
#	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/root/httpserver

clean:
	rm -rf httpserver *.a *.log *.so
