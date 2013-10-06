all: filter

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o filter

push: clean
	git add ./
	git commit --allow-empty-message -m ""
	git push origin master
