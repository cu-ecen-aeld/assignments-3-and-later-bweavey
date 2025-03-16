CC := gcc

ifeq ($(CROSS_COMPILE),)
	CC := gcc
else
	CC := $(CROSS_COMPILE)gcc
endif

all: aesdsocket 

aesdsocket: aesdsocket.o
	$(CC) aesdsocket.o -o aesdsocket 

aesdsocket.o: aesdsocket.c
	$(CC) -c aesdsocket.c -o aesdsocket.o

clean:
	rm -f aesdsocket aesdsocket.o
