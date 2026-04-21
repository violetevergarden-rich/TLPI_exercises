#define MAX_ALLOCS (1000000)
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>

int 
getInt(char *str, int base)
{
    // 检查是否为空指针或空串
    if(str == NULL || *str == '\0'){
        fprintf(stderr, "This str is NULL\n");
        exit(EXIT_FAILURE);
    }
    long res; 
    char *endptr;

    errno = 0;
    res = strtol(str, &endptr, base);
   
    // 检查base（以什么进制读取）是否合法
    if(errno == EINVAL){
        fprintf(stderr, "Base contains an unsupported value\n"), exit(EXIT_FAILURE);
    }
    // 检查是否超过long的范围
    if(errno == ERANGE){
        fprintf(stderr, "The resulting value was out  of range.\n"), exit(EXIT_FAILURE);
    }

    // 检查是否有数字
    if(endptr == str){
        fprintf(stderr, "There is no digit\n"), exit(EXIT_FAILURE);
    }

    // 拒绝非全字匹配，防止静默截断
    if(*endptr != '\0'){
        fprintf(stderr, "Trailing garbage\n"), exit(EXIT_FAILURE);
    }

    // 检查返回值是否在int类型里面

    if( res < INT_MIN || res > INT_MAX){
        fprintf(stderr, "The resulting value was out of range int\n"), exit(EXIT_FAILURE);
    }

    return (int)res;

}

int
main(int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    if(argc < 3 || strcmp(argv[1], "--help") == 0){
        fprintf(stderr, "Usage: %s num-allocs block-size [step[min[max]]]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numAllocs = getInt(argv[1], 10);
    if(numAllocs > MAX_ALLOCS){
        fprintf(stderr, "numAllocs > %d\n",MAX_ALLOCS);
        exit(EXIT_FAILURE);
    }

    blockSize = getInt(argv[2], 10);

    freeStep = (argc > 3) ? getInt(argv[3], 10) : 1;
    freeMin  = (argc > 4) ? getInt(argv[4], 10) : 1;
    freeMax  = (argc > 5) ? getInt(argv[5], 10) : numAllocs;

    if(freeMax > numAllocs){
        fprintf(stderr, "free-max > num-allocs\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Initial program break: %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

    for(j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        printf("Program break is now: %10p\n", sbrk(0));
        if(ptr[j] == NULL){
            fprintf(stderr, "malloc");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Program break is now: %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n",
            freeMin, freeMax, freeStep);

    for(j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is :%10p\n", sbrk(0));

    exit(0);

}
