all: write read filter

write: write_pol.c lib.o
	gcc -o write write_pol.c lib.o

read: read_pol.c lib.o
	gcc -o read read_pol.c lib.o

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o write read filter

push: clean
	git add *
	git commit --allow-empty-message -m ""
	git push origin master
