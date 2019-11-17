#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"


int main(){

    struct Direction *d = (struct Direction*) malloc(sizeof(struct Direction));            
    int c  = sizeof(d->next_entry_direction);
    printf("the size is %d\n",c);
    
    return 0;
}