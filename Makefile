all: filter netfilter getmac config

netfilter: netfilter.c lib.o
	gcc -o netfilter -lnfnetlink -lnetfilter_queue netfilter.c lib.o

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

getmac: getmac.c lib.o
	gcc -o getmac getmac.c lib.o

config: config.c creds.c lib.o
	gcc -o config `mysql_config --cflags` `mysql_config --libs` config.c lib.o

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o filter netfilter getmac config

push: clean
	git commit -a --allow-empty-message -m "${MSG}"
	git push origin master
