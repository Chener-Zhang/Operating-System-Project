#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "disk.h"


int main(){
    printf("\n\n");     

    make_disk("mydisk");    
    open_disk("mydisk");
    block_write(1,"hello world \n");
    block_write(2,"my name is chener\n");
    close_disk();    
    
    printf("\n\n");
    return 0;
}