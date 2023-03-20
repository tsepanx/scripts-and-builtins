

#// This is makefile.c file
#a : client.o server.o gcc
#        client.o server.o client.o : client.o server.h gcc
#    - c client.c server.o : server.o server.h gcc
#    - c server.c


all: clean exec
# all: clean single 

exec: main.o utils.o
	gcc -Wall $^ -o $@

main.o: main.c # main.h
	gcc -Wall $< -c -o $@

utils.o: funcs.c funcs.h
	gcc -Wall $< -c -o $@

clean:
	rm -f *.o *.out

single: single.c
	gcc single.c -o single
