#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/mman.h>
#include <time.h>
#include "struct.h"




// --------------------------------------------- Global Var ---------------------------------------------//

int number_of_block = 100;
int each_block_size = 64;
int fd;
char command[50];
char argument[50];


// Direction Information ---------
int current_index_block = 1;
int previous_direction_block = 2;
int direction_used_block = 3;
int n_things_inside_block = 4;
// File Information ------

int below_direction_block = 5;
int used_block = 6;
int first_block_entry_block = 7;

// Name Information ---- 
int direction_name_block = 8;
int file_name_block = 9;
// --------------------------------------------- Global Var ---------------------------------------------//
int super_block = 0 ; // for disk_split() function;
int data_block_entry_index = 32 ; // for disk_split() function;
struct Direction *dirtable[direction_list]; 
struct File *filetable[file_list]; 
struct Block *blocktable[block_list];
struct Direction *traking_dir;

// --------------------------------------------- Global Var ---------------------------------------------//


// --------------------------------------------- Main function---------------------------------------------//
int main(){


    begin();
    return 0;
}
// --------------------------------------------- Main function---------------------------------------------//

int begin(){
        // open a disk 
        char name[] = "disk";
        open_disk(name);      
        lseek(fd, 0 , SEEK_SET);
        //set the pointer    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        int new_int = atoi(read_buffer);
        // check whether the disk has open before or not 
        if(new_int == 1){         
            init_dir(dirtable);
            init_root(dirtable);
            init_file(filetable);    
            init_block(blocktable);
            loading(dirtable,filetable);                  
        }
        else
        {
        // DISK INITING ----------------------
            Mount(name);    
            write_disk(super_block,"1");
            write_disk(data_block_entry_index,"Data block");
        // FILE INITING ----------------------
            init_dir(dirtable);
            init_root(dirtable);
            init_file(filetable);    
            init_block(blocktable);           
        }      

        // while loop
        printf("You are current in the root now\n");
        printf("type: help\n");
        while(1)
            {
                // parsing shell stuff
                parsing();
                if(strcmp(command,"quit") == 0){
                    printf("bye - bye : > \n");
                    break;
                }else{
                    if(strcmp(command,"ls") == 0){
                        print_direction(traking_dir,dirtable,filetable);
                    }else if(strcmp(command,"cd") == 0){
                        Change_directory(argument,traking_dir,dirtable);
                    }else if(strcmp(command,"mkdir")== 0){
                        Create_directory(argument,dirtable,traking_dir);
                    }else if(strcmp(command,"rmr") == 0){
                        Delete_directory(argument,dirtable,traking_dir,filetable);
                    }else if(strcmp(command,"touch") == 0){
                        Create_file(argument,traking_dir,dirtable,filetable);
                    }else if(strcmp(command,"rm") == 0){
                        Delete_file(argument,dirtable,traking_dir,filetable);
                    }else if(strcmp(command,"write") == 0){
                        Write_file(argument,traking_dir,dirtable,filetable,blocktable);
                    }else if(strcmp(command,"read") == 0){
                        Read_file(argument,dirtable,traking_dir,filetable);
                    }else if(strcmp(command,"help") == 0){
                        help();
                    }
                    else{
                        printf("checking your input\n");                
                    }        
                }
                char_reset();        
            }
            // saving the data from the previous data;                  
            updating_metablock(dirtable,filetable);
            // End of while loop
            // close the disk    
            Unmount(fd);
            printf("\n\n");
            return 0;
        }


// --------------------------------------------- Some Useful function for myself---------------------------------------------//

// initializing the file[]
int help(){
    //Direction cmd
    printf("Command for dir\n");
    printf("Type [ls] ----  view current direction, example : ls \n");
    printf("Type [cd] [dirname] ----  change direction, example : cd dirname \n");
    printf("Type [mkdir] [dirname] ----  create a direction, example : mkdir dirname \n");
    printf("Type [rmr] [dirname] ----  remove a direction, example : rmr dirname \n");
    //File cmd
    printf("\n");
    printf("Command for File\n");
    printf("Type [touch] [filename] ----  create a file, example : touch filename \n");
    printf("Type [rm] [filename] ----  remove a file, example : rm filename \n");
    printf("Type [write] [filename] ----  write a file, example : write filename \n");
    printf("Type [read] [filename] ----  read a file, example : read filename \n");
    printf("\n");
    printf("Other tips: \n");
    printf("cd .. will be go to the previous direction \n");    
    return 0;
}

// initializing the file
int init_file(struct File *list[]){
    for (int i = 0; i < file_list; i++)
    {
        list[i] = (struct File*) malloc(sizeof(struct File));
        memset(list[i]->name, 0, sizeof *list[i]->name);
        list[i]->below_direction = -1;
        list[i]->used = 0;
    }    
    return 0;
}

// initializing the blocks[]
int init_block(struct Block *list[]){ 
    // for the block table; - > FAT ALLOCATION ALGORIHEM
    for (int i = 0; i < file_list; i++)
    {
        list[i] = (struct Block*) malloc(sizeof(struct Block));
        list[i]->next_block = -1; // list[0] = -1 = NULL / do not have the next, next is nothing;
        list[i]->used = 0; // check if used
        list[i]->is_full = 0; // check if it full
        list[i]->size_remain = each_block_size; // check the remain size
    }    

    //init the first block    
    list[0]->next_block = -1; // if next block is avalaible
    list[0]->used = 1; // check is used
    list[0]->is_full = 1; // check if fulled
    list[0]->size_remain = 0; // check the size remain

    // if everything workds fine return 0
    return 0;
    }

//Print my current table list for debug;
int print_list(){
    for (int i = 0; i < direction_list; i++)
    {
        printf("%d:[%d] |",i,dirtable[i]->current_index);
    }
    printf("\n");
    return 0;
}

// basically the shell thing; PARSING!!!
int parsing(){
    char user_input[each_block_size];
    char copy[each_block_size];
    printf(">"); 
    fgets(user_input,sizeof(user_input),stdin); 
    strcpy(copy, user_input); 
    int user_input_len = (int)strlen(user_input);
    if(user_input_len == 1){
        return 0;
    }
    strcpy(command,strtok(user_input," \n")); 
    int command_len =(int)strlen(command);            
    if(command_len == user_input_len - 1){
        return 1;
    }else if (command_len == user_input_len - 2){
        return 1;
    }else{
        strcpy(argument,strtok(copy," \t\n"));
        strcpy(argument,strtok(NULL," \t\n"));      
        return 1;
    }
}
// -------------Done with the shell thing------------------->

// reset my char[] clean my space // 
int char_reset(){
    memset(command, 0, sizeof(command));
    memset(argument, 0, sizeof(command));
    return 0;
}

//print out the direction easy for my testing and tracking
void print_direction( struct Direction *dir,struct Direction *list[],struct File *file_table[]){    
    printf("\tName\t \n");
    for (int i = 0; i < direction_list; i++)
    {
        if(list[i]->previous_index == dir->current_index)
        {
            printf("\t%s \t\t %s\n", list[i]->name, list[i]->time_of_creation);
        }

    }

    for (int i = 0; i < file_list; i++)
    {                
        if(dir->current_index == file_table[i]->below_direction){
            printf("\t%s \t\t %s\n", file_table[i]->name,file_table[i]->time_of_creation);    
        }
    }    
}

// Get the free space + return the list if it has the space;
int get_free_space_dirtable(struct Direction *list[]){ // dir
    for (int i = 1; i < direction_list; i++)
    {        
       if(list[i]->used == 0){
           return i;
       }
    }
    printf("The space is full now\n");    
    return -1;
}



// Get the free space + return the list if it has the space;
int get_free_space_filetable(struct File *list[]){ 
    for (int i = 1; i < direction_list; i++)
    {        
       if(list[i]->used == 0){
           return i;
       }
    }
    printf("The space is full now\n");    
    return -1;
}

// Get the free space + return the list if it has the space;
int get_free_space_blocktable(struct Block *list[]){
       for (int i = 1; i < block_list; i++)
    {    
       if(list[i]->used == 0){           
           return i;
       }       
    }
    printf("The space is full now\n");    
    return -1;
}

// --------------------------------------------- Some Useful function for myself Finished ---------------------------------------------//

// initilization of direction struct for all;
int init_dir(struct Direction *list[]){
    for (int i = 0; i < direction_list; i++)
    {
        list[i] = (struct Direction*) malloc(sizeof(struct Direction));
        memset(list[i]->name, 0, sizeof *list[i]->name);
        list[i]->previous_index = -1;
        list[i]->n_things_inside = 0;
    }
    return 0;
}

// Initilization for my Root
int init_root(struct Direction *list[]){
    strcpy(list[0]->name,"root");
    list[0]->current_index = 0;
    list[0]->previous_index = -1; 
    list[0]->used = 1;
    traking_dir = list[0];
    return 0;
}

// --------------------------------------------- Partition a disk ---------------------------------------------//
int Mount(char *name){
    create_disk(name);
    open_disk(name);
    return 0;
}

int Unmount(int fd){
    close_disk(fd);
    return 0;
}
// --------------------------------------------- Create a disk ---------------------------------------------//
int create_disk(char *name ){
    int file_size_counter;
    fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);    
    if(fd < 0){
        printf("creation fail\n");
        return -1;
    }
    char buf[each_block_size];
    memset(buf, 0, each_block_size);

    for (file_size_counter = 0;  file_size_counter < number_of_block; file_size_counter++)
    {
                write(fd, buf, each_block_size);
    }

    close(fd);
    return 0;
  
}

// --------------------------------------------- Open a disk ---------------------------------------------//
int open_disk(char *name){
    fd = open(name,O_RDWR,0644);
    if(fd < 0)
    {
        printf("creation fail\n");
        return -1;
    }
    return 0;
}

// --------------------------------------------- Write a disk/block ---------------------------------------------//
int write_disk(int block_index, char* words){      
    lseek(fd, block_index * each_block_size, SEEK_SET);
    write(fd,words,strlen(words));
    if(write < 0){
        printf("write fail\n");
        return -1;
    }
    return 0;
}
// --------------------------------------------- Read a disk/block ---------------------------------------------//

// 0 - > NULL, 1 - > HAS SOMETHING
int read_disk(int block_index){
    lseek(fd, block_index * each_block_size, SEEK_SET);
    if(lseek < 0){
        printf("lseek fail\n");
        return -1;
    }
    char read_buffer[each_block_size];
    read(fd,read_buffer,each_block_size);
    //printf("[%s]\n",read_buffer);
    return 0;  
}

// --------------------------------------------- Delete a disk/block ---------------------------------------------//

int delete_block(int block_index){
    lseek(fd, block_index * each_block_size, SEEK_SET);
    if(lseek < 0){
        printf("lseek fail\n");
        return -1;
    }
    char buf[each_block_size];    
    memset(buf, 0, each_block_size);
    write(fd, buf, each_block_size);    
    if(write < 0){
        printf("write fail\n");
        return -1;
    }
    return 0;
}
// --------------------------------------------- Close a disk ---------------------------------------------//
int close_disk(int fd){
    close(fd);
    if(close < 0){
        printf("close fail\n");
        return -1;
    }
    return 0;
}






// --------------------------------------------- File function------------------------------------------------------------------------//


// --------------------------------------------- Create a File---------------------------------------------//
int Create_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[]){
    //Get the free space for data block
    int position = get_free_space_filetable(file_table);


    if(position < 0){
        perror("fail to create direction\n");
        return -1;
    }

    for (int i = 0; i < file_list; i++)
    {
            // check if within the current direction            
            if(file_table[i]->below_direction == current_dir->current_index) 
            {
                // check if the file exit
                if(strcmp(file_table[i]->name,filename) == 0 )
                {
                    printf("you cannot create the file has same name as %s\n",filename);
                    return -1;                    
                }        
            }
    }
    // if pass the checker, then init the value for the file;
    strcpy(file_table[position]->name,filename);
    file_table[position]->below_direction = current_dir->current_index;   
    file_table[position]->used = 1;
    memcpy(filename,file_table[position]->name,each_block_size-1); 

    int current_id = current_dir->current_index;   
    dir_table[current_id]->n_things_inside ++;    
    //Success return 0;
    return 0;
}


// --------------------------------------------- Write a File---------------------------------------------//
int Write_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[],struct Block *block_table[])
{            
    for (int i = 0; i < file_list; i++)
    {                                                    
        //check the current path for the file i want to write;
        if(file_table[i]->below_direction == current_dir->current_index)
        { 
        // check the file exist
        if(strcmp(file_table[i]->name,filename) == 0 )
        {  
            char user_input[1000]; 
            fgets(user_input,sizeof(user_input),stdin);                 
            int free_block_id_data = get_free_space_blocktable(block_table);                
            int tracking_current_block = free_block_id_data;                
            if(free_block_id_data == -1)
            {
                    printf("Allocation fail\n");
                    return -1;
            }
            else
            {                    
                    int user_input_len = (int)strlen(user_input);                                          
                    int blocks_need = user_input_len/each_block_size;
                    int remainder = user_input_len % each_block_size; 
                    char buffer[each_block_size];          
                    memcpy(buffer, user_input,each_block_size - 1);

                    // first entry -- init                                         
                    blocktable[free_block_id_data]->used = 1;                                                              
                    file_table[i]->first_block_entry = free_block_id_data;                                      
                    write_disk(data_block_entry_index + free_block_id_data,buffer); 
                    // if the words < size of each block
                    if(blocks_need == 0)
                    { 
                        block_table[free_block_id_data]->next_block = -1;
                    }
                        

                    else                        
                    {                                                                                                        
                    // ------- block 2 - n allocation -- free_block_id_data --- begin 
                    // get index from the below loop                    
                    int end_index_from_loop;
                    for (int i = 1; i < blocks_need; i++)
                    {            
                            int index_start = i * each_block_size;
                            int index_end = index_start + each_block_size;            
                            memcpy(buffer, &user_input[index_start],each_block_size - 1);                                                                                                                       
                            // connecting block begin
                            int free_space = get_free_space_blocktable(block_table);                                                            
                            block_table[tracking_current_block]->next_block = free_space;
                            block_table[free_space]->used = 1;
                            tracking_current_block = free_space;                                
                            write_disk(data_block_entry_index + tracking_current_block,buffer);                               
                            end_index_from_loop = index_end;
                    }                                                    

                    if(remainder == 0)
                    {
                        return 0;
                    }
                    // ------- block 2 - n allocation -- free_block_id_data --- Finished;
                        
                    else                                
                    {
                    // ------- last block for remaind words - n allocation -- free_block_id_data                                    
                        int free_space = get_free_space_blocktable(block_table); 
                        block_table[tracking_current_block]->next_block = free_space;
                        block_table[free_space]->used = 1;
                        tracking_current_block = free_space;
                        block_table[tracking_current_block]->next_block = 0;
                        memcpy(buffer, &user_input[end_index_from_loop],each_block_size - 1);                            
                        write_disk(data_block_entry_index + free_space,buffer);                                                        
                    }                         

                printf("\n");                     
                }     
                return 0;
                }

            
            } 
        }            
    }
    
    printf("The file does not exit \n");
    return 0;         
}


// --------------------------------------------- Read a File---------------------------------------------//
int Read_file(char *filename, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]){
  for (int i = 0; i < file_list; i++)
    {
            if(file_table[i]->below_direction == current_dir->current_index)
            {   

                if(strcmp(filename,file_table[i]->name)==0 )
                {
                    int current = file_table[i]->first_block_entry;
                    //printf("current %d\n",current);                            
                    for (int i = 0; i < block_list; i++)
                    {
                        if(current == i)
                        {   

                            printf("\n");
                            read_disk(data_block_entry_index + current);
                            current = blocktable[i]->next_block;                           
                        }                                                                               
                    }                                        
                    return 1;
                }
            }
    }        
    return 0;
}

// --------------------------------------------- Delete a File---------------------------------------------//
int Delete_file(char *filename, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]){
    
    for (int i = 0; i < file_list; i++)
    {
        if(file_table[i]->below_direction == current_dir->current_index)
        {   
            if(strcmp(filename,file_table[i]->name)==0 )
            {
                memset(file_table[i]->name,0,sizeof(file_table[i]->name));                            
                file_table[i]->below_direction = -1;
                file_table[i]->used = 0;      

     
                int current = file_table[i]->first_block_entry;
                int previous = file_table[i]->first_block_entry;
                for (int i = 0; i < block_list; i++)
                {
                    if(current == i)
                    {                                                        
                        printf("\n");                                                                                
                        delete_block(data_block_entry_index + current);                           
                        current = blocktable[i]->next_block;                                                    
                        blocktable[previous]->next_block = 0;                        
                        blocktable[previous]->used = 0;                            
                        previous = current;                                                
                    }                                                                               
                }                                                
            dir_table[current_dir->current_index]->n_things_inside --;            
            return 0;
            }
        }
    }    
    printf("Invalid enter\n");
    return -1;
}


// --------------------------------------------- Create a Direction---------------------------------------------//

int Create_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir){
    //check if exist
    int position = get_free_space_dirtable(dir_table);
    if(position < 0){
        perror("fail to create direction");
        return -1;
    }
    for (int i = 0; i < direction_list; i++)
    {          
            if(dir_table[i]->previous_index == current_dir->current_index)
            {
                if(strcmp(dirname,dir_table[i]->name)==0 )
                {
                perror("you cannot create a dir with same name \n");
                return -1;
                }
            }
    }     
       
       
        strcpy(dir_table[position]->name,dirname);
        dir_table[position]->current_index = position;
        dir_table[position]->previous_index = current_dir->current_index;
        dir_table[position]->used = 1;    
        dir_table[current_dir->current_index]->n_things_inside++;    
        return 1;
}


// --------------------------------------------- Change a Direction---------------------------------------------//

int Change_directory(char *dirname,struct Direction *current_dir, struct Direction *dir_table[]){        
    if (strcmp(dirname,"..") == 0 ){        
        if(current_dir->current_index == 0)
        {
            printf("It is the root, you cannot go previous\n");
            return -1;
        }
        else
        {
            int previous_dir = current_dir->previous_index; // redire the current
            traking_dir = dir_table[previous_dir]; // go to the previosu dir
            return 1;
        }     
    }

    for (int i = 0; i < direction_list; i++)
    {
            if(dir_table[i]->previous_index == current_dir->current_index)
            { 
                    if(strcmp(dirname,dir_table[i]->name)==0 )
                    {
                        traking_dir = dir_table[i];
                        return 0;
                    }
            }

    }
    printf("Direction does not exits \n");
    return -1;
}


// --------------------------------------------- Delete a Direction ---------------------------------------------//

int Delete_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]){
    for (int i = 0; i < direction_list; i++)
    {
        if(dir_table[i]->previous_index == current_dir->current_index)
        {                                
            if(strcmp(dirname,dir_table[i]->name) == 0 )
            {
            if(dir_table[i]->n_things_inside > 0){                             
                printf("You have something inside of this direction, Delete fail\n");
                return -1;
            }else
                    {                                                
                       
                        memset(dir_table[i]->name,0,sizeof(dir_table[i]->name));                            
                        dir_table[i]->current_index = 0;
                        dir_table[i]->previous_index = -1;
                        dir_table[i]->used = 0;                                     
                        dir_table[i]->n_things_inside = 0;
                        return 1;
                    }                                                       
            }
        printf("Direction does not exit\n");
        return -1;
        }
    }    
    return 0;
}


// ADDITIONAL FEATURE --------------------------------------------------------------------------------------------------------
// NEW ELEMENT FOR MEDA BLOCK FUNCTION 
int updating_metablock(struct Direction *dirlist[], struct File *filelist[]){
    // Direction meta block initilization ------------------->

    for (int i = 1; i < 8; i++)
    {
        delete_block(i);
    }


    // current index block 1
    int counter_current_index_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", dirlist[i]->current_index);
        write_metablock_special(current_index_block,i +counter_current_index_block ,str);
        memset(str, 0, sizeof(str));
        counter_current_index_block +=3;
    }
   

    
    // current index block 2
    int counter_previous_direction_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", dirlist[i]->previous_index);
        write_metablock_special(previous_direction_block,i + counter_previous_direction_block ,str);
        memset(str, 0, sizeof(str));
        counter_previous_direction_block +=3;
    }
    
    // dir used block 3
    int counter_direction_used_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", dirlist[i]->used);
        write_metablock_special(direction_used_block,i + counter_direction_used_block ,str);
        memset(str, 0, sizeof(str));
        counter_direction_used_block +=3;
    }

    // n_thing_in_dir_block 4
    int counter_n_things_inside = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", dirlist[i]->n_things_inside);
        write_metablock_special(n_things_inside_block,i + counter_n_things_inside ,str);
        memset(str, 0, sizeof(str));
        counter_n_things_inside +=3;
    }

    // Direction meta block Update Finish ------------------->

    // File meta block initilization ------------------->


    //below_direction_block = 5;
    int counter_below_direction_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", filelist[i]->below_direction);
        write_metablock_special(below_direction_block,i + counter_below_direction_block ,str);
        memset(str, 0, sizeof(str));
        counter_below_direction_block +=3;
    }

    //int used_block = 6;
    int counter_used_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", filelist[i]->used);
        write_metablock_special(used_block,i + counter_used_block ,str);
        memset(str, 0, sizeof(str));
        counter_used_block +=3;
    }

    //int first_block_entry_block = 7;
    int counter_first_block_entry_block = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%d", filelist[i]->first_block_entry);
        write_metablock_special(first_block_entry_block,i + counter_first_block_entry_block ,str);
        memset(str, 0, sizeof(str));
        counter_first_block_entry_block +=3;
    }
    
    // File meta block Update Finish ------------------->



    // direction_name_block = 8;
    int dir_counter = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%s", dirlist[i]->name);
        write_metablock_special(direction_name_block,i + dir_counter ,str);
        memset(str, 0, sizeof(str));
        dir_counter+= 20;
    
    }
    // int file_name_block = 9;
    int file_counter = 0;
    for (int i = 0; i <  direction_list; i++)
    {
        char str[10];
        sprintf(str, "%s", filelist[i]->name);
        write_metablock_special(file_name_block,i + file_counter ,str);
        memset(str, 0, sizeof(str));
        file_counter += 20;
    }

    // Name block finish init;

    return 0;
}

int write_metablock_special(int block_index,int special, char item[]){

    lseek(fd, block_index * each_block_size + special, SEEK_SET);
    write(fd,item,strlen(item));

    if(write < 0){
        printf("write fail\n");
        return -1;
    }
    return 0;
}


int loading(struct Direction *dirtable[], struct File *filetable[]){

    // current_index_block 1
    for (int i = 0; i < direction_list; i++)
    {        
        lseek(fd, current_index_block * each_block_size + i*4 , SEEK_SET);   
        char read_buffer[each_block_size];        
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);        
        int new_int = atoi(read_buffer);               
        dirtable[i]->current_index = new_int;         
    }
    
        //printf("\n");


    // int previous_direction_block = 2;
    for (int i = 0; i < direction_list; i++)
    {
        lseek(fd, previous_direction_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);

        int new_int = atoi(read_buffer);       
        dirtable[i]->previous_index = new_int; 
        //printf("the len is %d\n",new_int);

    }
     printf("\n");

    //int direction_used_block = 3;
    for (int i = 0; i < direction_list; i++)
    {
         lseek(fd, direction_used_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));

    //printf("[%s]",read_buffer);

        int new_int = atoi(read_buffer);       
        dirtable[i]->used = new_int; 
        //printf("the len is %d\n",new_int);

    }
    //printf("\n");


    //int n_things_inside_block = 4;
    for (int i = 0; i < direction_list; i++)
    {
        lseek(fd, n_things_inside_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
    //printf("[%s]",read_buffer);
        int new_int = atoi(read_buffer);       
        dirtable[i]->n_things_inside = new_int; 
    //printf("the len is %d\n",new_int);

    }

    //printf("\n");
  


    // File   -------
    
    for (int i = 0; i < file_list; i++)
    {
        lseek(fd, below_direction_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);
        int new_int = atoi(read_buffer);       
        filetable[i]->below_direction = new_int; 
        //printf("the len is %d\n",new_int);
    }
  
     // printf("\n");

    //int used_block = 6;
    for (int i = 0; i < file_list; i++)
    {
         lseek(fd, used_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);
        int new_int = atoi(read_buffer);       
        filetable[i]->used = new_int; 
        //printf("the len is %d\n",new_int);

    }
    printf("\n");
    printf("\n");
    //int first_block_entry_block = 7;
    for (int i = 0; i < file_list; i++)
    {
        lseek(fd, first_block_entry_block * each_block_size + i*4 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);
        int new_int = atoi(read_buffer);       
        filetable[i]->first_block_entry = new_int; 
        //printf("the len is %d\n",new_int);

    }
    //printf("\n");
    
    
    // Name
    //int direction_name_block = 8;
      for (int i = 0; i < file_list; i++)
        {
        lseek(fd, direction_name_block * each_block_size + i*21 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);
             
        strcpy(dirtable[i]->name,read_buffer);
      //printf(" [%s]",dirtable[i]->name);

        }
    //printf("\n");
    
    //int file_name_block = 9;

         for (int i = 0; i < file_list; i++)
        {
        lseek(fd, file_name_block * each_block_size + i*21 , SEEK_SET);    
        char read_buffer[each_block_size];
        read(fd,read_buffer,sizeof(read_buffer));
        //printf("[%s]",read_buffer);             
        strcpy(filetable[i]->name,read_buffer);
    //printf("[%s]",filetable[i]->name);

        }
    //printf("\n");
    return 0;
}