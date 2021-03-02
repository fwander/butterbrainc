CC = gcc
RM = rm
RM += -f

all:
	$(CC) butterbrainc.c -o bbc -Wall
	chmod +x ./bbc

clean:
	$(RM) ./bbc
