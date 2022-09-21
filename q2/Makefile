CC = gcc
CFLAGS = -Wall
OBJS = q2.o
EXEC = addmx

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
