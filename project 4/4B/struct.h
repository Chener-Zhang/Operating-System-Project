#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   
#define bloc_entry_number  20   
#define file_list_number 10


struct File { 
    char name[name_size]; 
    int block_entry[]; 
    // add additional imformation here
};

struct Direction{
    // name of the direction
    char name[name_size];
    struct File* file_list[file_list_number];
    struct Direction* next_entry_direction[];
    // add additional imformation here
};

struct Block{
    int index;
    int size;
    // add additional imformation here
};
