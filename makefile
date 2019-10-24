CC = gcc

ifeq ($(OS),Windows_NT) 
RM = del /Q /F
CP = copy /Y
ifdef ComSpec
SHELL := $(ComSpec)
endif
ifdef COMSPEC
SHELL := $(COMSPEC)
endif
else
RM = rm -rf
CP = cp -f
endif

s1: s1.o strbuf.o
	$(CC) -o s1 s1.o strbuf.o

s1.o : s1.c strbuf.h
	$(CC) --std=c11 -ggdb -c s1.c
strbuf.o : strbuf.c strbuf.h
	$(CC) --std=c11 -ggdb -c strbuf.c

macro.o: macro.c util.h
	$(CC) --std=c11 -ggdb -c macro.c

test: test.o gc.o util.o buildin.o slice.o type.o map.o hash.o *.c *.h
	$(CC) -o test --std=c11 -ggdb *test*.c util.o type.o gc.o buildin.o slice.o map.o hash.o

test.o: test.c test.h gc.h util.h buildin.h slice.h map.h type.h *test*.c
	bash test_before.sh
	$(CC) --std=c11 -ggdb -c test.c

util.o: util.c util.h 
	$(CC) --std=c11 -ggdb -c util.c

type.o: type.c type.h 
	$(CC) --std=c11 -ggdb -c type.c
	
gc.o: gc.c gc.h util.h  type.h
	$(CC) --std=c11 -ggdb -c gc.c

slice.o: slice.c slice.h type.h gc.h util.h buildin.h
	$(CC) --std=c11 -ggdb -c slice.c

hash.o: hash.c hash.h type.h gc.h util.h buildin.h hash.h
	$(CC) --std=c11 -ggdb -c hash.c

map.o: map.c map.h type.h gc.h util.h buildin.h hash.h
	$(CC) --std=c11 -ggdb -c map.c

buildin.o: buildin.c buildin.h 
	$(CC) --std=c11 -ggdb -c buildin.c

clean :
	$(RM) s1 test test.o s1.o strbuf.o macro.o slice.o gc.o util.o buildin.o type.o