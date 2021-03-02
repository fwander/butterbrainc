CC = gcc
RM = rm
RM += -f

build:
	$(CC) butterbrainc.c -o bbc 
	chmod +x ./bbc

clean:
	$(RM) ./bbc
