#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define name_size  10   //define
#define bloc_entry_number  30   // for calculation
#define direction_list 30  // [#] plugin number 
// DIY the var here:
#define file_list 30     // [#] plugin number 
#define block_list 30   // [#] plugin number 
// Note from myself: Assignment requirement;  
// 16,384 blocks total     ---- 2^14
// 8,192 for data blocks   ----- 2^13
// 8191 for meda -- 2^13 - 1
// 1 for super   --- 2^0
/*
just for testing easy to read;
super - > 1;
meta -  > 32 --- 2^5;
data - > 32 ---> 2^5;
*/
//-------------------------------------------------------------------------------------------------------->
struct File;
struct Direction;
struct Block;
struct File { 
    char name[name_size]; //index of the block which contains the imformation of the file;
    int below_direction;
    int size;
    int used;
    int first_block_entry;
    int meta_block_entry;

    //done
    // int next_block_entry;    
};

struct Direction{    
    char name[name_size];// name of the direction    
    int current_index;
    int previous_index;   
    int used;   
    int n_things_inside;  
    int meta_block_entry;
    // add additional imformation here
};
struct Block{
    int next_block; // the id / the index of the block
    int size_remain; // the size of a single block remain 
    int is_full;
    int used;     
};
// ---------------- Function declares --------------------------- 
int char_reset();
int parsing();