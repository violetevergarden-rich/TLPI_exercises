#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#define MAX_READ 4096

int main(int argc,char *argv[]){
    

    if((argc < 3 || strcmp(argv[1],"--help")) == 0){
        exit(0);
    }

    int source_fd = open(argv[1],O_RDONLY,0);
    int target_fd = open(argv[2],O_WRONLY | O_TRUNC | O_CREAT,0644);

    char buf[MAX_READ];
    ssize_t numbers_read;
     
    while((numbers_read = read(source_fd,buf,MAX_READ)) > 0){
        
        bool allZeros = true;
        for(int i=0;i<numbers_read;i++){
            if(buf[i] != '\0'){
                allZeros = false;            
            }   
        
        }
        
    if(allZeros){
        // 如果是空洞，跳过这个块
        lseek(target_fd,numbers_read,SEEK_CUR);
    }
    else{
        write(target_fd,buf,numbers_read);
    }
    }

    
    
     

}
