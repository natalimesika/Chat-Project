CFLAG=-c -ansi -Wall -g  
CC=gcc
INCS=-I../ListenChat
OBJ=ListenerProgram.o


all_listen: listen

listen: $(OBJ) 
	$(CC) -o listen.out $(OBJ)

ListenProgram.o: ../ListenChat/ListenerProgram.c 
	$(CC) $(CFLAG) $(INCS) ../ListenChat/ListenerProgram.c 
