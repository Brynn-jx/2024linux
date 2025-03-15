#include "head.h"
//请输入用户名和密码
int input_username_passwd(user_passwd_t *user_passwd){
    printf("username:\n");
    scanf("%s",user_passwd->username);
    printf("passwd:\n");
    scanf("%s",user_passwd->password);
    return 0;
}

char* split_space(char *cmd) {
    int len = strlen(cmd);
    printf("%s\n", cmd);

    // 计算新字符串所需的最大长度
    int newLen = len + 1; // 假设至少需要跟原字符串一样长的空间，+1是为了'\0'
    char *p = malloc(newLen); // 分配足够的空间
    if (p == NULL) return NULL; // 检查内存分配是否成功

    int j = 0;
    for(int i = 0; i < len; i++) {
        // 跳过所有前导空格
        while(i < len && cmd[i] == ' ') i++;
        // 复制直到下一个空格或字符串结束
        while(i < len && cmd[i] != ' ') {
            p[j++] = cmd[i++];
        }
        if(i < len || j > 0) { // 如果不是在开头且前面有非空格字符，则添加一个空格
            p[j++] = ' ';
        }
    }

    if(j > 0 && p[j-1] == ' ') j--; // 移除最后可能多余的空格
    p[j] = '\0'; // 添加字符串终止符

    return p;
}

