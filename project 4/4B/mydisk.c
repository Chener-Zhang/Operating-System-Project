#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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
    lseek(fd, 0, SEEK_SET);    
    lseek(fd, block_index * each_block_size, SEEK_SET);    
    char read_buffer[each_block_size];
    read(fd,read_buffer,strlen(read_buffer));
    printf("%s",read_buffer);
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
    char name[] = "disk";

    create_disk(name);
    open_disk(name);
    write_disk(0,"one");
    write_disk(1,"two");
    write_disk(2,"three");
    write_disk(4,"three");


    close_disk(fd);

    printf("\n\n\n");
}