cc = gcc
cxx = g++
cflags = -I./LocationFinder/LocationFinder
ldflags = -L./LocationFinder/x64/Debug
libs = -lLocationFinder

test:
	$(cc) $(cflags) -c test.c -o test.o
	$(cc) $(ldflags) test.o -o test.exe $(libs)