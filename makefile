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

BDO= util.o type.o gc.o slice.o  map.o hash.o str.o buildin.o  print.o 
BDC= util.c type.c gc.c slice.c  map.c hash.c str.c buildin.c  print.c 

s1: s1.o strbuf.o
	$(CC) -o s1 s1.o strbuf.o

s1.o : s1.c strbuf.h
	$(CC) --std=c11 -ggdb -c s1.c
strbuf.o : strbuf.c strbuf.h
	$(CC) --std=c11 -ggdb -c strbuf.c

macro.o: macro.c util.h
	$(CC) --std=c11 -ggdb -c macro.c

ShuntingYard: ShuntingYard.c $(BDO) csv.o
	$(CC) -o ShuntingYard --std=c11 -ggdb ShuntingYard.c $(BDO) csv.o

csv: csv.o csv.h $(BDO)
	$(CC) -o csv csv.o $(BDO)

csv.o: csv.c csv.h 
	$(CC) --std=c11 -ggdb -c csv.c 

test: *.o  *.c *.h
	$(CC) -o test --std=c11 -ggdb *test*.c $(BDO)

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

str.o: str.c str.h type.h gc.h util.h buildin.h
	$(CC) --std=c11 -ggdb -c str.c

hash.o: hash.c hash.h type.h gc.h util.h buildin.h hash.h
	$(CC) --std=c11 -ggdb -c hash.c

map.o: map.c map.h type.h gc.h util.h buildin.h hash.h
	$(CC) --std=c11 -ggdb -c map.c

buildin.o: buildin.c buildin.h 
	$(CC) --std=c11 -ggdb -c buildin.c

print.o: print.c print.h buildin.h
	$(CC) --std=c11 -ggdb -c print.c

clean :
	$(RM) s1 *.o