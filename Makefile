#!/bin/bash

send:udpsend.o udprecv.o
	gcc udpsend.o -o send
	gcc udprecv.o -o QUESTION
udpsend.o:udpsend.c
	gcc -c udpsend.c
udprecv.o:udprecv.c
	gcc -c udprecv.c
clean:
	rm *.o
