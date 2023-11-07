#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void 
primes(int read_fd)
{
    int prime; 
    int p[2];
    pipe(p);
    if(read(read_fd, &prime, sizeof(int)) > 0) {
        printf("prime %d\n", prime);
        if (fork() == 0) {
            close(p[1]);
            primes(p[0]);    
        } else {
            close(p[0]);  // 父进程关闭读入端, 防止xv6文件描述符不够
            int num;
            while (read(read_fd, &num, sizeof(int)) > 0) {
                if (num % prime != 0) {
                write(p[1], &num, sizeof(int));
                }
            }
            close(p[1]);
            wait(0);
        }
    }    
}

int 
main(int argc, char *argv[])
{
    if (argc != 1) {
        fprintf(2, "User: primes");
        exit(1);
    } else {
        int pip[2];
        pipe(pip);
        if (fork() == 0) {
            close(pip[1]);
            primes(pip[0]);
        } else {
            close(pip[0]);
            for (int i = 2; i <= 35; i++) {
                write(pip[1], &i, sizeof(int));
            } 
            close(pip[1]);
            wait(0);
        }

    }
    exit(0);
}

