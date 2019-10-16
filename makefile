s1: s1.o strbuf.o
	gcc -o s1 s1.o strbuf.o

s1.o : s1.c strbuf.h
	gcc --std=c99 -ggdb -c s1.c
strbuf.o : strbuf.c strbuf.h
	gcc --std=c99 -ggdb -c strbuf.c

clean :
	rm s1 s1.o strbuf.o