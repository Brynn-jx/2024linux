#include<func.h>

int sendFd(int pipeFd, int fd){ // 将文件描述符通过套接字发送给子线程，这个过程在内核中完成制定的是发送的规则
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));

    char buf[] = "Hello";
    struct iovec iov[1];    // 通常用于支持高效的 I/O 操作（如读写操作），尤其是在需要一次性处理多个不连续的内存块时
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;

    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;

    // 控制信息的部分
    struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1, CMSG_LEN(sizeof(int)));
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); //CMSG_LEN 会自动计算出所需要占用的内存大小，这里只传送一个文件描述符，所以只需要一个int 
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(pcmsghdr) = fd; // CMSG_DATA获取cmsghdr中data的地址，数据部分是文件描述符
    hdr.msg_control = pcmsghdr; // hdr的辅助数据段
    hdr.msg_controllen = CMSG_LEN(sizeof(int));

    int ret = sendmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}

int recvFd(int pipeFd, int* fd){
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));

    char buf[6] = {0};
    struct iovec iov[1];    // 通常用于支持高效的 I/O 操作（如读写操作），尤其是在需要一次性处理多个不连续的内存块时
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;

    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;

    // 控制信息的部分
    struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1, CMSG_LEN(sizeof(int)));
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); //CMSG_LEN 会自动计算出所需要占用的内存大小，这里只传送一个文件描述符，所以只需要一个int 
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;

    hdr.msg_control = pcmsghdr; // hdr的辅助数据段
    hdr.msg_controllen = CMSG_LEN(sizeof(int));

    int ret = recvmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    *fd = *(int *)CMSG_DATA(pcmsghdr);
    return 0;
}