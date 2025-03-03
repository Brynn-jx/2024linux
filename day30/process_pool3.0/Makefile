SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
# CFLAGS:=-g
ELF:=main
$(ELF):$(OBJS)
	gcc -o $@ $^	  

rebuild: clean $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
