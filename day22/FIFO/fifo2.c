#include<func.h>
#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
    ARGS_CHECK(argc, 2);
    // 打开命名管道，以只读模式
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    // 读取管道中的数据
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
    ERROR_CHECK(bytes_read, -1, "read");
    // 在读取的数据末尾添加字符串结束符
    buffer[bytes_read] = '\0';
    // 输出读取的数据
    printf("Read from pipe: %s\n", buffer);
    // 关闭文件描述符
    close(fd);
    // 删除命名管道文件
    unlink(argv[1]);
    return 0;
}