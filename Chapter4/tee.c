#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAX_READ 20

int main(int argc,char *argv[]){
    int opt;
    int flags = O_CREAT | O_WRONLY;
    int is_append = 0;
    char buf[MAX_READ];
    
    if (argc < 2 || strcmp(argv[1], "--help") == 0){
        exit(0);
    }

    /*
        设置是否为append模式
    */    

    while((opt = getopt(argc,argv,"a")) != -1){
        switch (opt){
            case 'a':
                is_append = 1;
                break;
            default :
                fprintf(stderr,"Usage: %s [-a] file\n",argv[0]);
                exit(1);
        }
    }
   
    if(is_append){
        flags |= O_APPEND;
    }
    else{
        flags |= O_TRUNC;
    } 

    if(optind >= argc){
        fprintf(stderr,"can't find filename");
    }

    char *filename = argv[optind];
    int filefd = open(filename,flags,0644);

    ssize_t numRead;

    while((numRead = read(STDIN_FILENO,buf,MAX_READ)) > 0){
        
        if(write(STDOUT_FILENO,buf,numRead) != numRead){
            perror("write stdout");
            exit(1);
        }

        if(write(filefd,buf,numRead) != numRead){
            perror("write file");
            exit(1);
        }
    }
    
    

}
