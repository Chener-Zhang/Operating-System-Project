#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   
#define bloc_entry_number  20   
#define file_list_number 10


struct File { 
    char name[name_size]; //index of the block which contains the imformation of the file;
    int block_entry[]; // add additional imformation here
    
};

struct Direction{    
    char name[name_size];// name of the direction    
    struct File* file_list[file_list_number]; // file list here
    struct Direction* next_entry_direction[]; // directory list here
    // add additional imformation here
};

struct Block{
    int index; // the id / the index of the block
    int size_remain; // the size of a single block remain 
    // add additional imformation here
};