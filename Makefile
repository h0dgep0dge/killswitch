all: clean filter nfqueue getmac

nfqueue: netfilter.c lib.o
	gcc -o nfqueue -lnfnetlink -lnetfilter_queue netfilter.c lib.o

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

getmac: getmac.c
	gcc -o getmac getmac.c

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o filter test nfqueue getmac

push: clean
	git commit -a --allow-empty-message -m "${MSG}"
	git push origin master
