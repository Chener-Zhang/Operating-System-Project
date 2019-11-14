#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "disk.h"


int main(){
    printf("\n\n");    
    char name[] = "mydisk";
    make_disk(name);
    open_disk(name);
    block_write(0,"hello world");
    close_disk(name);

    printf("\n\n");
    return 0;
}