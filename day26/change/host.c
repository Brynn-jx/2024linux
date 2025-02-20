#include<func.h>

/**
 * 
struct hostent {
    char *h_name; /* official name of host
    char **h_aliases; 别名列表 
    int h_addrtype; ipv4 ipv6 
    int h_length;  
    char **h_addr_list;  IP地址  存储的是 IP 地址的二进制形式
 */
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    struct hostent *pHost;
    pHost = gethostbyname(argv[1]);     
    printf("%s\n", pHost->h_name);
    printf("%d %d\n",pHost->h_addrtype, pHost->h_length);
    for(int i = 0; pHost->h_aliases[i] != NULL; i++){
        printf("alias %s\n", pHost->h_aliases[i]);
    }

    char buf[100];
    for(int i = 0; pHost->h_addr_list[i] != NULL; i++){
        memset(buf, 0, sizeof(buf));
        printf("addr %s\n", inet_ntop(pHost->h_addrtype, pHost->h_addr_list[i], buf, sizeof(buf)));
    }
    
    return 0;
}