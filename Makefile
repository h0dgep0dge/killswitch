all: filter

test: netfilter.c
	gcc -o test netfilter.c -lnfnetlink -lnetfilter_queue

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
