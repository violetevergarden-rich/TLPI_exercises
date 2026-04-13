#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
int
my_dup(int oldfd)
{
    fcntl(oldfd, F_DUPFD, 0);
}

int
my_dup2(int oldfd, int newfd)
{
    // 特殊情况检查

    if(fcntl(oldfd, F_GETFD) == -1){
        errno = EBADF;
        return -1;
    }

    if(oldfd == newfd){
        return newfd;
    }
    
    close(newfd);
    
    return fcntl(oldfd, F_DUPFD, newfd);

}

int
main(int argc, char *argv[])
{

    // ---------------- 
    // 测试 my_dup
    // ---------------- 
    int fd1 = open("test_dup.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    int fd2 = my_dup(fd1);
    
    write(fd1, "Hello ", 6);
    write(fd2, "World!\n", 7);
    
    close(fd1);
    close(fd2);
    // ---------------- 
    // 测试 my_dup2 正常    
    // ---------------- 

    // 打开文件，保存stdout
    int fd3 = open("test_dup2.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    int save_stdout = my_dup(STDOUT_FILENO);
    
    // 将stdout重定向到fd3
    
    if(my_dup2(fd3, STDOUT_FILENO) == -1){
        errExit("my_dup2");
    }
    // 在原来stdout的位置输入文字
    printf("这段消息会出现在文件里面，但是标准输出上面没有!\n");
    fflush(stdout);
    
    // 恢复stdout
    my_dup2(save_stdout, STDOUT_FILENO);
    
    // ---------------- 
    // 测试 my_dup2 异常 
    // ---------------- 
    int bad = my_dup2(999, 1);
    if(bad == -1 && errno == EBADF){
        printf("错误检测成功\n");
    }
    else{
        printf("错误检测失败\n");
    }


    return 0;
}
