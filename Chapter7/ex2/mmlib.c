#include <stdio.h>
#include "memlib.h"
#define WSIZE 4 // 一个字的大小
#define DSIZE 8 // 双字的大小
#define CHUNKSIZE (1<<12) // 一次扩展的malloc长度 
   
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *)(p))
#define SET(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) ( GET(p) & (~0x7))
#define GET_ALLOC(p) ( GET(p) & 0x1) 

#define GET_HEAD(bp) ( (char *)(bp) - WSIZE)
#define GET_FOOT(bp) ( (char *)(bp)+ GET_SIZE(GET_HEAD(bp)) - DSIZE )

#define PRE_BLKS(bp) ( (char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define NEX_BLKS(bp) ( (char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

static char *heap_listp = 0;

int mm_init(void);
static void *extend_heap(size_t words);
void mm_free(void *bp);
static void *coalesce(void *bp);
void *mm_malloc(size_t size);
static void *find_fit(size_t asize);
static void place(char *bp, size_t asize);

int 
mm_init(void)
{
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1){
        return -1;
    }

    SET(heap_listp, 0); // 填充块
    SET(heap_listp + (WSIZE),   PACK(DSIZE, 1)); // 序言块头
    SET(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); // 序言块尾
    SET(heap_listp + (3*WSIZE), PACK(0, 1)); // 结尾块                                              

    heap_listp += WSIZE;

    if(extend_heap(CHUNKSIZE/WSIZE) == NULL){
        return -1;
    }

    return 0;
}

static void
*extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words%2) ? (words + 1) * WSIZE : (words) *WSIZE;
    if((bp = mem_sbrk(size)) == (void *)-1){
        return NULL;
    }

    SET(GET_HEAD(bp), PACK(size, 0));
    SET(GET_FOOT(bp), PACK(size, 0));
    SET(GET_HEAD(NEX_BLKS(bp)), PACK(0, 1));

    return coalesce(bp);
}

void
mm_free(void *bp)
{
    size_t size = GET_SIZE(GET_HEAD(bp));

    SET(GET_HEAD(bp), PACK(size, 0));
    SET(GET_FOOT(bp), PACK(size, 0));
    
    coalesce(bp);
    
}

static void
*coalesce(void *bp)
{
    size_t pre_alloc = GET_ALLOC(GET_FOOT(PRE_BLKS(bp)));
    size_t nex_alloc = GET_ALLOC(GET_HEAD(NEX_BLKS(bp)));
    size_t size = GET_SIZE(GET_HEAD(bp));

    if(pre_alloc && nex_alloc){
        return bp;
    }
    else if(!pre_alloc && nex_alloc){
        size += GET_SIZE(GET_FOOT(PRE_BLKS(bp)));
        SET(GET_HEAD(PRE_BLKS(bp)), PACK(size, 0));
        SET(GET_FOOT(PRE_BLKS(bp)), PACK(size, 0));
        return GET_HEAD(PRE_BLKS(bp));
    }
    else if(pre_alloc && !nex_alloc){
        size += GET_SIZE(GET_FOOT(NEX_BLKS(bp)));
        SET(GET_HEAD(bp), PACK(size, 0));
        SET(GET_FOOT(bp), PACK(size, 0));
        return GET_HEAD(NEX_BLKS(bp));
    }
    else if(!pre_alloc && !nex_alloc){
        size += GET_SIZE(GET_FOOT(PRE_BLKS(bp))) + GET_SIZE(GET_HEAD(NEX_BLKS(bp)));
        SET(GET_HEAD(PRE_BLKS(bp)), PACK(size, 0));
        SET(GET_FOOT(bp), PACK(size, 0));
        return GET_HEAD(PRE_BLKS(bp));
    } 
    return bp;
}

void
*mm_malloc(size_t size)
{
    size_t asize;
    size_t extend_size;
    char* bp;

    if(size == 0){
        return 0;
    }

    if(size <= DSIZE){
        asize = 2*DSIZE; // 16字节，多出来的8字节用来放head和foot
    }
    else{
        asize = (size + DSIZE + (DSIZE - 1) & (~(DSIZE-1)));
    }

    if((bp = find_fit(asize)) != NULL){
       place(bp, asize);
       return bp;
    }
    
    extend_size = MAX(asize, CHUNKSIZE);

    if((bp = extend_heap(extend_size/WSIZE)) == NULL){
        return NULL;
    }

    place(bp, asize);

    return bp;

}

// first fit
static void
*find_fit(size_t asize)
{
    void *bp;
    for(bp = heap_listp; GET_SIZE(GET_HEAD(bp)) != 0; bp = NEX_BLKS(bp)){
        if(!(GET_ALLOC(GET_HEAD(bp))) && (asize <= GET_SIZE(GET_HEAD(bp)))){
            return bp;
        }
    }
    return NULL;
}

static void
place(char *bp, size_t asize)
{
    size_t remain_size = GET_SIZE(GET_HEAD(bp)) - asize;
    if(remain_size >= DSIZE){
        SET(GET_HEAD(bp), PACK(asize, 1));
        SET(GET_FOOT(bp), PACK(asize, 1));
        SET(GET_HEAD(NEX_BLKS(bp)), PACK(remain_size, 0));
        SET(GET_FOOT(NEX_BLKS(bp)), PACK(remain_size, 0));
    }
}

