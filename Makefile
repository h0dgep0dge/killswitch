all: filter nfqueue getmac getconf cli_getmac cli_config

nfqueue: netfilter.c lib.o
	gcc -o nfqueue -lnfnetlink -lnetfilter_queue netfilter.c lib.o

filter: filter.c lib.o
	gcc -o filter -lipq filter.c lib.o

getmac: getmac.c
	gcc -o getmac getmac.c

getconf: getconf.c
	gcc -o getconf getconf.c

cli_getmac: cli_getmac.c lib.o
	gcc -o cli_getmac cli_getmac.c lib.o

cli_config: cli_config.c creds.c lib.o
	gcc -o cli_config `mysql_config --cflags` `mysql_config --libs` cli_config.c lib.o

lib.o: lib.h lib.c
	gcc -c lib.c

clean:
	rm -f *.o filter test nfqueue getmac getconf cli_getmac cli_config

push: clean
	git commit -a --allow-empty-message -m "${MSG}"
	git push origin master
