################ Makefle #################
all : server client serv_for_vlc

S_OBJS += server.o src/serv_process.o 
V_OBJS += serv_for_vlc.o src/serv_process.o 

server : $(S_OBJS)
	gcc -I include $(S_OBJS) -o $@ -L lib -lrecord_parse

client : $(C_OBJS)

# lib_test : lib_test.o
# 	gcc -o lib_test lib_test.o -I include -L lib -lrecord_parse

serv_for_vlc : $(V_OBJS)
	gcc -I include $(V_OBJS) -o $@ -L lib -lrecord_parse

%.o:%.c
	gcc -c $^ -o $@

clean:
	-@rm -f *.o server client serv_for_vlc
