CC = gcc
CFLAGS = -Wall
OBJS = q1.o
EXEC = phrases

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
