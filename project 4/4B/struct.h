#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   
#define bloc_entry_number  20   
#define file_list_number 10
#define direction_list_number 10


struct File { 
    char name[name_size]; //index of the block which contains the imformation of the file;
    int block_entry[bloc_entry_number]; // add additional imformation here
    
};

struct Direction{    
    char name[name_size];// name of the direction    
    int current_index;
    int previous_index;        
    // add additional imformation here
};

struct Block{
    int index; // the id / the index of the block
    int size_remain; // the size of a single block remain 
    // add additional imformation here
};
