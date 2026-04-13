#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int 
main(int argc, char *argv[])
{
    int fd;
    
    if(argc == 4){
        fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    }
    else{
        fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, 0644);
    }

    ssize_t numWrite;
    int numBytes = atoi(argv[2]);
    for(int i=0;i<numBytes;i++){

        if(argc == 4){
            if(lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");        
        }
        if((numWrite = write(fd, "G", 1)) != 1){
            errExit("write");
        }
    }


}
