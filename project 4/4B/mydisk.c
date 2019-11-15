#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "struct.h"

int number_of_block = 20;
int each_block_size = 16; 
int fd;
// --------------------------------------------- Create a disk ---------------------------------------------//
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

// --------------------------------------------- Open a disk ---------------------------------------------//
int open_disk(char *name){
    fd = open(name,O_RDWR,0644);
    return 0;
}

// --------------------------------------------- Write a disk/block ---------------------------------------------//
int write_disk(int block_index, char* words){
      
    lseek(fd, block_index * each_block_size, SEEK_SET);
    write(fd,words,strlen(words));    
    return 0;
}
// --------------------------------------------- Read a disk/block ---------------------------------------------//

int read_disk(int block_index){
    lseek(fd, block_index * each_block_size, SEEK_SET);    
    char read_buffer[each_block_size];
    read(fd,read_buffer,strlen(read_buffer));
    printf("%s",read_buffer);
    return 0;
}
// --------------------------------------------- Delete a disk/block ---------------------------------------------//

int detele_block(int block_index){
    lseek(fd, block_index * each_block_size, SEEK_SET);  

    char buf[each_block_size];
    memset(buf, 0, each_block_size);
    write(fd, buf, each_block_size);
    printf("\n this is the buf [%s]\n",buf);
    
    return 0;
}
// --------------------------------------------- Close a disk ---------------------------------------------//
int close_disk(int fd){
    close(fd);
    return 0;
}

// --------------------------------------------- Main ---------------------------------------------//
int main(){ 

    printf("\n\n\n");
    struct my_file* item = (struct my_file*) malloc(sizeof(struct my_file));
    

    printf("\n\n\n");
    return 0;
}