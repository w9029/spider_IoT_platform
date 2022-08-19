gcc -g -Wall -I../ -DWITH_POSIX -c newserver.c -o newserver.o
gcc newserver.o -o newserver -L../ -lcoap -I../
