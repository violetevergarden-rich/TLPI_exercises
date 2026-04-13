#include "tlpi_hdr.h"
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd;
    off_t off;
    
    if(argc != 2 || strcmp(argv[1], "--help" )== 0)    
        usageErr("%s filename\n",argv[0]);

    fd = open(argv[1] , O_RDWR | O_CREAT | O_APPEND, 0644);
    
    if(lseek(fd,0,SEEK_SET) == -1)
        errExit("lseek");
    if(write(fd, "test", 4) == -1) 
        errExit("write");
    close(fd);
   
    return 0;

}
