#include <sys/types.h>  /* 类型定义，如 pid_t, size_t */
#include <sys/stat.h>   /* 文件状态、权限常量 */
#include <fcntl.h>      /* 文件控制定义，如 O_CREAT, O_RDWR */
#include <stdio.h>      /* 标准 I/O */
#include <stdlib.h>     /* 标准库函数，如 exit, malloc */
#include <unistd.h>     /* 系统调用，如 read, write, close, dup */
#include <errno.h>      /* 错误号定义 */
#include <string.h>     /* 字符串处理，如 strerror */

/* 如果你用了 TLPI 提供的库 */
#include "tlpi_hdr.h"

void
print_file_content(const char* label)
{
    char buf[100];
    int fd = open("file", O_RDWR);
    if(fd == -1)
        errExit("open");

    ssize_t num_read = read(fd, buf, sizeof(buf) - 1);
    
    printf("[%s]: ", label);
    if(num_read > 0){
        printf("%s\n", buf);
    }
    else{
        printf("(empty)\n");
    }

}

int
main(int argc, char *argv[])
{
    int fd1 = open("file", O_RDWR | O_CREAT | O_TRUNC | S_IRUSR | S_IWUSR, 0644);
    int fd2 = dup(fd1);
    int fd3 = open("file", O_RDWR);
    
    write(fd1, "Hello,", 6);
    print_file_content("第一次写入Hello");
    
    write(fd2, "world", 6);
    print_file_content("写入world");

    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    print_file_content("写入HELLO");

    write(fd3, "Gidday", 6);
    print_file_content("写入Gidday");

    return 0;
}
