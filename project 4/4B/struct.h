#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


struct my_file{
    char file_name[50];
    int size;
    int blocks_index[];
    
};

struct my_direction{
    char directory_name[50];    
    int direction_fat_table[];
};

