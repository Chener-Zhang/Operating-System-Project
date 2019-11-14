#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int number_of_block = 20;
int each_block_size = 16; 
int fd;


int create_disk(char *name ){

    int file_size_counter;
    fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buf[each_block_size];
    memset(buf, 0, each_block_size);


    for (file_size_counter = 0;  file_size_counter < number_of_block; file_size_counter++){
            write(fd, buf, each_block_size);
    }
    
    close(fd);
    return 0;
}

int main(){ 
    create_disk("hello");
}