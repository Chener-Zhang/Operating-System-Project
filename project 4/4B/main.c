#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "disk.h"


int main(){
    printf("\n\n");        
    make_disk("mydisk");
    //open_disk("mydisk");
    //block_write(1,"hello world \n");
    //close_disk("mydisk");
    
    printf("\n\n");
    return 0;
}