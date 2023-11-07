#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;
    // 利用逆向找到文件名
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
    p++;

    // 
    if(strlen(p) >= DIRSIZ)
        p[DIRSIZ] = '\0';
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void
find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de; // 目录结构
    struct stat st;   // 文件结构

    // 错误评定
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    //
    switch(st.type){
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }  
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(de.name, ".") == 0 
                    || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0) {
                    fprintf(2, "find: cannot stat %s\n");
                    continue;
                }
                if(st.type == T_DIR){
      				find(buf, filename);
      			}else if (st.type == T_FILE){
      				if (strcmp(de.name, filename) == 0)
      				{
      					printf("%s\n", buf);
      				}
                }
            }
            break;
        case T_FILE:
            if(strcmp(fmtname(path), filename) == 0){
                printf("%s\n", path);
            }
            break;
    }
    close(fd);
}

int 
main(int argc, char *argv[]) 
{
    if(argc != 3) {
        fprintf(2, "User: find [path] [filename]\n");
        exit(1);
    }
    char *path = argv[1];
    char *filename = argv[2];
    find(path, filename);
    exit(0);
}