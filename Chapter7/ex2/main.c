#include <stdio.h>
#include "memlib.h"
#include "mmlib.h"

int
main()
{
    mem_init();
    mm_init();

    int n;
    scanf("%d",&n);

    int *arr = (int *)mm_malloc(sizeof(int) * n);

    for(int i=0;i<n;i++){
        printf("%d\n",i);
    }

    mm_free(arr);

    return 0;
}
