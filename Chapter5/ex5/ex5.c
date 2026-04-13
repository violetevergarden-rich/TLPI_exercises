#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int
main()
{
    // 验证共享文件偏移
    int fd1 = open("test_off.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
    int fd2 = dup(fd1);
    
    // 没有使用append打开，如果不共享，那么应该是world!\n 
    write(fd1, "Hello ", 6);
    write(fd2, "World!\n", 7);
    
    close(fd1);
    close(fd2);
    
    // 试试不使用dup，而是直接让fd2指向test_unoff.txt会怎么样
    int fd3 = open("test_unoff.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
    int fd4 = open("test_unoff.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
    
    write(fd3, "Hello ", 6);
    write(fd4, "World!\n", 7);
    
    // 验证共享打开文件标志
    
    int fd5 = open("test_flag.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
    int fd6 = dup(fd5);
    
    int flag5 = fcntl(fd5, F_GETFL);
    printf("Before flag5 (append) is = %d\n", (flag5 & O_APPEND) != 0);

    int flag6 = fcntl(fd6, F_GETFL);
    printf("Before flag6 (append) is = %d\n", (flag6 & O_APPEND) != 0);

    fcntl(fd5, F_SETFL, flag5 | O_APPEND);

    flag6 = fcntl(fd6, F_GETFL);

    printf("After flag6 (append) is = %d\n", (flag6 & O_APPEND) != 0);
     

    
}
