#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("child pid = %d ppid = %d\n", getpid(), getppid());
        sleep(5);
        exit(0);
    }else{
        int status; 
        pid_t result;
        do {
            result = waitpid(pid, &status, WNOHANG);
            if(result == 0){
                printf("No child has exited yet. Waiting...\n");
                sleep(1);
            }            
        } while (result == 0);
        if(result == pid){
            if(WIFEXITED(status)){
                printf("Child exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }
    return 0;
}
