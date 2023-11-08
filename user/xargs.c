#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXLEN 100
int
main(int argc, char *argv[]) 
{
    if(argc <= 1) {
        fprintf(2, "usage: xargs command (arg...)\n");
        exit(1);
    }
    char *command = argv[1];
    char buf;
    char new_argv[MAXARG][MAXLEN]; // assuming the maximun single parameter length is 512
    char *p_new_argv[MAXARG];

    while(1) {
        memset(new_argv, 0, MAXARG * MAXLEN); // reset the parameter

        for(int i = 1; i < argc; ++i) {
            strcpy(new_argv[i-1], argv[i]);
        }

        int cur_argc = argc - 1;
        int offset = 0;
        int is_read = 0;

        while((is_read = read(0, &buf, 1)) > 0) {
            if(buf == ' ') {
                cur_argc++;
                offset = 0; 
                continue;
            }
            if(buf == '\n') {
                printf("%d", is_read);
                break;
            }
            if(offset==MAXLEN) {
                fprintf(2, "xargs: parameter too long\n");
                exit(1);
            }
            if(cur_argc == MAXARG) {
                fprintf(2, "xargs: too many arguments\n");
                exit(1);
            }
            new_argv[cur_argc][offset++] = buf;
        }
        if(is_read <= 0) {
            break;
        } 
        for(int i = 0; i <= cur_argc; ++i) {
            p_new_argv[i] = new_argv[i];
        }
        
        if(fork() == 0) {
            exec(command, p_new_argv);
            exit(1);
        } 
        wait((int*) 0);
    }
    exit(0);
}

// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/param.h"

// #define MAXLINE 128
// int 
// main(int argc, char *argv[])
// {
//     if(argc < 2){
//         fprintf(2, "User: xargs [command] ...\n");
//         exit(1);
//     }
//     // 取指令和参数
//     char *com = argv[1];
//     char *args[MAXARG];
//     char buf[MAXLINE];
//     for(int i = 1; i < argc; i++){
//         args[i-1] = malloc(strlen(argv[i])+1);
//         strcpy(args[i-1], argv[i]);
//     }

//     // Read
//     int r = 0;
//     int argCount = argc - 1;
//     int tol = 0;
//     int statr = 0;
//     //char block[32];
//     int m = 0;
//     while((r = read(0, buf, sizeof(buf))) > 0){
//         // 错误 
//         if(r >= MAXLINE - 1){
//             fprintf(2, "xargs: argument too long\n");
//             exit(1);
//         }
//          for(int i = 0; i < r; i++){
//             // 如果是换行，不优化
//             if(buf[i] == '\n'){
//                 m = r;
//                 printf("%d\n", m);
//                 break;
//             }
//             // 如果是空格，说明是一个参数
//             if (buf[i] == ' ') {
//                 tol++;
//             }
//         }

//     }  
//     printf("%d\n", m);
    
//     if(m <= 0){
//         fprintf(2, "xargs: no argument\n");
//         exit(1);
//     }
//     buf[m] = 0; // 休止符
//     //printf("%s\n", buf);
//     // 写入参数
    
//     for(int j = argCount; j < argCount+tol; ++j){
//         if(buf[statr] == ' '){
//             buf[statr++] = 0;
//             statr = 0;
//         }else{ 
//             statr++;
//         }
//         args[j] = buf + statr++;
//     }
//     argCount += tol;
    
//     args[argCount] = 0;
//     //printf("%d\n", argCount);
//     // for(int k = 0; k < argCount; k++){
//     //     printf("%s\n", args[k]);
//     // }
//     // printf("%s\n", com);
//     // child
//     if(fork() == 0){
//         exec(com, args);
//         fprintf(2, "xargs: exec failed\n");
//         exit(1);
//     }else{
//         wait(0);
//     }
    
//     exit(0);
// }