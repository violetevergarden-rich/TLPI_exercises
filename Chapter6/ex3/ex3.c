#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 100

extern char** environ;


int
env2name_value(const char *env, char *name, char *value )
{
    // env为空指针，空串，=开头
    if(env == NULL || *env == '\0' || *env == '='){
        return -1;
    }
    // 寻找等号位置
    char *p = strchr(env, '=');
    if(p == NULL){
        return -1;
    }

    int name_len = p - env;
    memcpy(name, env, name_len);
    name[name_len] = '\0';

    int value_len = strlen(p+1);
    mempcpy(value, p+1, value_len+1);

    return 0;
}

int
my_setenv(const char *name, const char *value, int overwrite)
{
    // 检查name的合法性
    if(name ==NULL || *name == '\0' || strchr(name,'=') != NULL){
        perror("name");
        exit(EXIT_FAILURE);
    }

    // 格式串name = value
    size_t name_len = strlen(name);
    size_t value_len = strlen(value);
    size_t total_size = name_len + value_len + 2;
    char *name_equal_value = (char*)malloc(total_size);

    char *p = name_equal_value;

    mempcpy(p, name, name_len);
    p += name_len;

    *p++ = '=';

    mempcpy(p, value, value_len);
    p += value_len;
    *p = '\0';

    // 查找name是否在env中存在

    char *find_value = getenv(name);
    // 如果存在且overwrite为0
    if(find_value != NULL && overwrite == 0){
        return 0;
    }

    return putenv(name_equal_value);

}

int 
my_unsetenv(const char *name)
{
    
    if(name ==NULL || *name == '\0' || strchr(name,'=') != NULL){
        perror("name");
        exit(EXIT_FAILURE);
    }

    size_t name_len = strlen(name);
    char** ep = environ;
    char** sp;

    while(*ep != NULL){
        if(strncmp(*ep, name, name_len) ==0 && (*ep)[name_len] == '='){
            for(sp = ep; *sp != NULL; sp++){
                *sp = *(sp+1);
            }
        }
        else{
            ep++;
        }
    }

    return 0;
}
int
main(int argc, char* argv[])
{
    int opt;
    int longindex;
    char name_buf[MAX_BUF], value_buf[MAX_BUF];
    char* find_value;
    static struct option option_table[]= {
        {"getenv", 1, NULL, 'g'},
        {"setenv", 1, NULL, 's'},
        {"unset", 1, NULL, 'u'},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "g:s:u:", option_table,  &longindex)) != -1){
    
            switch(opt){
            case 's':
                if(env2name_value(optarg, name_buf, value_buf) == -1){
                    perror("非法格式");
                    exit(EXIT_FAILURE);
                }
                if(my_setenv(name_buf, value_buf, 1) == -1){
                    printf("Failed to set %s\n",name_buf);
                    perror("setnev");
                    exit(EXIT_FAILURE);
                }
                printf("Env var %s is set to %s\n", name_buf, value_buf);
            
            break;

            case 'g':
               if((find_value = getenv(optarg)) == NULL){
                   printf("Env %s is no exit\n", optarg);
               }
               printf("Env var %s value: %s\n",optarg, find_value);
              break;

            case 'u':
                if(my_unsetenv(optarg) == -1){
                   printf("Failer to unset %s\n",optarg);
                   perror("unset");
                   exit(EXIT_FAILURE);
                }
                else{
                   printf("Env var %s is unset\n", optarg);
                }
           break;
           }
    }
    

    return 0;
}
