all: filter test

test: netfilter.c lib.o
	gcc -o test -lnfnetlink -lnetfilter_queue netfilter.c lib.o

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o filter test

push: clean
	git add ./
	git commit --allow-empty-message -m "${MSG}"
	git push origin master
