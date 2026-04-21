#include <unistd.h>
#include <setjmp.h>

#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

void
Install_Jmp()
{
    switch(setjmp(env)){
        case 0: 
            printf("首次调用setjmp\n");
            break;
        case 1:
            printf("从longjmp跳回\n");
            break;

        }
}

void
try_Jmp()
{
    longjmp(env, 1);
}

void 
A_function_call()
{
    printf("这个函数已经返回了\n");
}

int 
main()
{
   Install_Jmp();

   A_function_call();

   try_Jmp();

}

