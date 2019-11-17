#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   
#define bloc_entry_number  20   


struct File { 
    char name[name_size]; 
    struct File* block_entry[]; 
};

struct Direction{
    char name[name_size];
    struct Direction* next_entry[];
};

struct Block{
    int index;
    int size;
};
