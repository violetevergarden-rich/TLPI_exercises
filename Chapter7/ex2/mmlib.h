#ifndef MMLIB_H
#define MMLIB_H

#include <stdio.h>

int mm_init(void);
static void *extend_heap(size_t words);
void mm_free(void *bp);
static void *coalesce(void *bp);
void *mm_malloc(size_t size);
static void *find_fit(size_t asize);
static void place(char *bp, size_t asize);

#endif
