#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
pingpong() 
{
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buf = '#';
    // parent
    if (fork() != 0) {
        
        if (write(p1[1], &buf, 1) == 1) {
            wait(0);
            printf("%d: received pong\n", getpid());
            read(p2[0], &buf, 1);  
            // exit(0);
        } else {
            fprintf(2, "Error: no read to child");
            exit(1);
        } 
        
    } else {
        // child
        if (read(p1[0], &buf, 1) == 1) {
            printf("%d: received ping\n", getpid());
            write(p2[1], &buf, 1);
            // exit(0);
        } else {
            fprintf(2, "Error: no read to parent");
            exit(1);
        }
    }
}

int 
main(int argc, char *argv[])
{
    if (argc == 1) {
        pingpong();
    } else {
        fprintf(2, "User: pingpong");
    }
    exit(0);
}

