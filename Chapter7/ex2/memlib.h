#ifndef MEMLIB_H
#define MEMLIB_H

#include <stddef.h>
#include <stdint.h>

void mem_init(void);
void *mem_sbrk(int incr);

#endif
