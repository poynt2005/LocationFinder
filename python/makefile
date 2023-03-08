cc = gcc
cxx = g++
cflags = -I../csdll/LocationFinder/LocationFinder -I"C:\Users\SteveChung\AppData\Local\Programs\Python\Python310\include"
ldflags = -L../csdll/LocationFinder/x64/Debug -L"C:\Users\SteveChung\AppData\Local\Programs\Python\Python310\libs"
libs = -lpython310 -lLocationFinder

pyd:
	$(cc) -fPIC -shared -DMS_W64 $(cflags) -c PyBinding.c -o PyBinding.o
	$(cc) -shared $(ldflags) PyBinding.o -o PyLocationFinder.pyd $(libs)