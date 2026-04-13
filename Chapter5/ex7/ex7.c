#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "tlpi_hdr.h"

ssize_t 
my_readv(int fd, const struct iovec *iov, int iovcnt)
{
    int numread;
    int total_size = 0;
    for(int i=0;i<iovcnt;i++){
        total_size += iov[i].iov_len;
    }
    if(total_size == 0)
        return 0;


    char *big = (char*) malloc(total_size);

    if(big == NULL)
        errExit("malloc");

    ssize_t bytes_read = read(fd, big, total_size);

    if(bytes_read == 0){
        free(big);
        return bytes_read;
    }

    ssize_t current_offset = 0;

    for(int i=0;i<iovcnt;i++){
         ssize_t copy_len = iov[i].iov_len;

         if(current_offset + copy_len > bytes_read){
             copy_len = bytes_read - current_offset;
         }

         mempcpy(iov[i].iov_base, big + current_offset, copy_len);

         current_offset += copy_len;

         if(current_offset >= total_size) break;

    }

    free(big);

    return bytes_read;

}

int 
main(int argc, char *argv[])
{
    
    int fd = open("test_readv.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

    write(fd, "ABCDEF123456", 12);

    lseek(fd, 0, SEEK_SET);

    char buf1[7] = {0};
    char buf2[7] = {0};
    
    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = 6;
    iov[1].iov_base = buf2;
    iov[1].iov_len = 6;
    
    ssize_t bytes_read = my_readv(fd, iov, 2);

    printf("The buf1 is %s\n", buf1);
    printf("The buf2 is %s\n", buf2);

}
