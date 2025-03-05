SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
CFLAGS:=-g 
ELF:=thread_pool
$(ELF):$(OBJS)
	gcc -o $@ $^ -lpthread

rebuild:clean $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
