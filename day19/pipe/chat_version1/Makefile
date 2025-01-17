SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:%.c=%)
CC:=gcc
CFLAGS:=-g -D ON
all:$(OBJS)
	@for i in $(OBJS); do $(CC) -o $${i} $${i}.c; done
rebuild:clean all
clean:
	rm $(OBJS)

