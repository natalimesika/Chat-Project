CFLAG=-c -ansi -Wall -g  
CC=gcc
INCS=-I../senderChat
OBJ=SenderProgram.o


all_sender: sender

sender: $(OBJ) 
	$(CC) -o sender.out $(OBJ)

SenderProgram.o: ../senderChat/SenderProgram.c 
	$(CC) $(CFLAG) $(INCS) ../senderChat/SenderProgram.c 
