#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   //define
#define bloc_entry_number  20   // for calculation
#define direction_list 10  // [#] plugin number 
#define file_list 10     // [#] plugin number 
#define block_list 10


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
    // int next_block_entry;    
};

struct Direction{    
    char name[name_size];// name of the direction    
    int current_index;
    int previous_index;   
    int used;     
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
int get_free_space_dirtable(struct Direction *list[]);
int get_free_space_filetable(struct File *list[]);
int init_dir(struct Direction *list[]);
int init_root(struct Direction *list[]);

int create_disk(char *name);
int open_disk(char *name);
int write_disk(int block_index, char* words);
int read_disk(int block_index);
int detele_block(int block_index);
int close_disk(int fd);

int init_block_allocation();
int init_block(struct Block *lisk[]);

int init_file(struct File *list[]);
int Create_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[]);
int Write_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[]);
int Read_file(char *filename);
int Delete_file(char *filename, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]);

int get_current_direction();
int Create_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
int Change_directory(char *dirname,struct Direction *current_dir, struct Direction *dir_table[]);
int Delete_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
void print_direction( struct Direction *dir,struct Direction *list[],struct File *file_table[]);
int print_list();
int begin();
// ---------------- Function declares ---------------------------

