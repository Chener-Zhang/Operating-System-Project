#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"





int main(){
    int list[10];
    for (int i = 0; i < 10; i++)
    {
        list[i] = 0;
//          printf("%d - > %d\n",i,list[i]);
    }
    
    for (int i = 0; i < 10; i++)
    {
          printf("%d - > %d\n",i,list[i]);
    }
    

    return 0;
}