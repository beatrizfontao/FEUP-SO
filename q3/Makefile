CC = gcc
CFLAGS = -Wall
OBJS = q3.o
EXEC = cypher

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
