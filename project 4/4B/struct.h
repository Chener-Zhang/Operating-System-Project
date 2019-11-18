#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



#define name_size  30   
#define bloc_entry_number  20   
#define direction_list 10
#define file_list 20



struct File { 
    char name[name_size]; //index of the block which contains the imformation of the file;
    int direction_below; // where the file below;
    int block_entry[bloc_entry_number]; // add additional imformation here
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
    int index; // the id / the index of the block
    int size_remain; // the size of a single block remain 
    // add additional imformation here
};
// ---------------- Function declares --------------------------- 
int char_reset();
int parsing();
void print_direction( struct Direction *dir,struct Direction *list[]);
int get_free_space(struct Direction *list[]);
int init_dir(struct Direction *list[]);
int init_root(struct Direction *list[]);
int disk_split();
int create_disk(char *name);
int open_disk(char *name);
int write_disk(int block_index, char* words);
int read_disk(int block_index);
int detele_block(int block_index);
int close_disk(int fd);

int Create_file(char *filename);
int Write_file(char* filename, char* words);
int Read_file(char *filename);
int Delete_file(char *filename);

int get_current_direction();
int Create_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
int Change_directory(char *dirname,struct Direction *current_dir, struct Direction *dir_table[]);
int Fine_directory(char *dirname);
int Delete_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
int print_list();
int begin();
// ---------------- Function declares ---------------------------

