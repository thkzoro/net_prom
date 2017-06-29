################ Makefle #################
all : server client lib_test

server : server.o
	gcc -o server server.o  -I include -L lib -lrecord_parse
server.o : server.c
	gcc -c -o server.o server.c

client : client.o
client.o : client.c
	gcc -c -o client.o client.c

lib_test : lib_test.o
	gcc -o lib_test lib_test.o -I include -L lib -lrecord_parse
lib_test.o : lib_test.c
	gcc -c -o lib_test.o lib_test.c  

clean:
	-@rm -f *.o server client lib_test