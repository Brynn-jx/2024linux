SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:%.c=%)
CC:=gcc
# 编译期标识 可以被隐式调用
CFLAGS:=-g -D ON
# 链接期标识 只有在链接阶段才会用到，因此它不需要在每一个规则中都声明 所以必须在式子中写出来 位置有影响要写在前面
LDFLAGS:=-pthread
all:$(OBJS)
	@for i in $(OBJS); do $(CC) $(LDFLAGS) -o $${i} $${i}.c -lpthread; done
rebuild:clean all
clean:
	rm $(OBJS)

