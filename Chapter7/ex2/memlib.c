#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#define MAX_HEAP (20 * 1024 * 1024) //定义虚拟堆的最大内存 20MB

static uint8_t *mem_heap; /* 堆的起始地址 */
static uint8_t *mem_brk; /* 堆顶 +1 */
static uint8_t *mem_max_addr; /* 堆的最大地址 */

void
mem_init(void)
{
    mem_heap = (uint8_t *)malloc(MAX_HEAP);
    if(mem_heap == NULL){
        fprintf(stderr, "malloc mem_heap");
        exit(EXIT_FAILURE);
    }
    mem_brk = (uint8_t *)mem_heap;
    mem_max_addr = (uint8_t *)(mem_heap + MAX_HEAP);
}

void 
*mem_sbrk(int incr)
{
    uint8_t *oldbrk = mem_brk;

    if( (incr < 0) || ( (mem_brk + incr) > mem_max_addr ) ){
        errno = ENOMEM; 
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *)-1;
    }

    mem_brk += incr;

    return (void *)oldbrk;
}
