# Project Description


### Before reading:
* **i m going to roughly explain my technic for making direction and file.
The data structure for making or changing the direction is very similar to
tree data structure; i have a current path and previous path in myh direction
structure. but i m not accuratly using the tree data structure; i combine the tree and using the array to represent my data something like heap representation;**

**SO:**
* **if directionX 's current direction == directionY's previous direction
    That means there is some relationship or connection between directionX 
    and directionY
    So, i can just using the for loop to check the array of direction struct;
    To figure out which direction's previous equals to one of the direction's current; if there is, just print them out;**



### **Define Global Var**


* ### **#define name_size  10**   
* ### **#define bloc_entry_number  10**   
* ### **#define direction_list 10**  
* ### **#define file_list 10**     
* ### **#define block_list 10**  




### Note for calculation

*  Note from myself: Assignment requirement;
*  16,384 blocks total     ---- 2^14
*  8,192 for data blocks   ----- 2^13
*  8191 for meda -- 2^13 - 1
* 1 for super   --- 2^0
* super - > 1;
* meta -  > 32 --- 2^5;
* data - > 32 ---> 2^5;

* ****
### **Function description**

### **struct File;**
* **Explanation:**
* **name: the name of the file**


* **below_direction: where the file located such as direction1, direction2,...so on;**


* **used: whether the file is created, in this case i want to check whether the file block is used; helping me to locate avalaible position for array;**


* **first_block_entry: FAT table first entry;**
* ****

```
struct File {
    char name[name_size]; 
    the file;
    int below_direction;
    int used;
    int first_block_entry;
};
```

### **struct Direction;**
* **Name: direction name**
* **current_index: the direction's current index**
* **previous_index: the direction's previous index or parent direction/ where it is belong**
* **used: whether the direction entry in the array is used or not; help to allocation the new direction space in the array**
* **n_thing_inside: number of stuff inside of the direction like how many files, directions, i used this to help me do "ls" command**
* ****


```
struct Direction{
    char name[name_size];
    int current_index;
    int previous_index;
    int used;
    int n_things_inside;
};
```

### **struct Block;**
```
struct Block{
    int next_block; // the id / the index of the block
    int size_remain; // the size of a single block remain
    int is_full;
    int used;
};
```

## **---------------- Function declares for shell---------------------------**
```
int char_reset(void);
```
### reset the char when i parsing the user input while using the shell

 

```
int parsing(void);
```
### parsing the user input

```
int help(void);
```
### help -- list the imformation u need you know


## **---------------- Function declares for disk operation----------------**
```
int Mount(char *name);
```
### create the disk, do the partition
```
int Unmount(int fd);
```
### Close the disk and reset
```
int get_free_space_dirtable(struct Direction *list[]);
```
### get the free index of array of direction; while the used == 1;
```
int get_free_space_filetable(struct File *list[]);
```
### get the free index of array of file; while the used == 1;
```
int init_dir(struct Direction *list[]);
```
### Initializing the direction array of direction;
```
int init_root(struct Direction *list[]);
```
### Initializing the root in the array[0] of direction;
```
int create_disk(char *name);
int open_disk(char *name);
int write_disk(int block_index, char* words);
int read_disk(int block_index);
int delete_block(int block_index);
int close_disk(int fd);
```
### Disk manipulation; which provide by the assignment; i change the function name,just wanna make it easy to read;


```
int init_block(struct Block *lisk[]);
```
### Initializing array of block;
```

int init_file(struct File *list[]);
```
### Initializing the array of file;
* ****


```
int Create_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[]);

int Write_file(char *filename, struct Direction *current_dir,struct Direction *dir_table[],struct File *file_table[],struct Block *block_table[]);

int Read_file(char *filename, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]);

int Delete_file(char *filename, struct Direction *dir_table[], struct Direction 
```
### **File manipulation Function**
* ### Create_file
 
   * 1: If the position is less than 0; which means the array of file position is *run out of the space;
   * 2: checking the file is in the correct position rather than in the werid place;
   * 3: making sure the name will not repeat; it repeat, print out the warning;
   * 4: if no error occurs, i create a file
   * 5: once the file is created, i set the attribution such as where this file is *belong to, what is the name;

* ### Write_file
    * 1: checking and making sure in the correct direction
    * 2: get the free data block and using the fat allocation algorithem
    * 3: i divide the block into three piece; I: first entry block; II second to  (n-1) block; III : N: the last block;
    * 4: the reason i divided the blocks into three piece is becasue i need to  calculate which each of the block is full or not; for the calculation
    * of the size;
    * 5: write the data to the data block and set the pointer from the meta block; * which need to update;
* ### Read_file

    * 1：Locating for the correction position
    * 2: using the read_disk which == read_block; put the array index which will contain the id of the block
    * 3: the read_block function will print the contain in side the disk for certain position;
* ### Delete_file
    * 1: Locating the correct space 
    * 2: the method i used here is actually same as write_block
    * 3: just over write the data into 0000 by using the memset
    * 4: updating the meta block imformation tracking

* ****


```
int Create_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
int Change_directory(char *dirname,struct Direction *current_dir, struct Direction *dir_table[]);
int Delete_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]);
void print_direction( struct Direction *dir,struct Direction *list[],struct File *file_table[]);
```
### **Direction manipulation Function**
* ### create_direction
  
  * 1:doing the same as the file array for checking the empty space;
  * 2:check whether it is in the correct direction
  * 3:check the name is not repeated;
  * 4: if everything works well,then init the direction of the array
  * 5:set the attribution for the direction arr;
  * 6:setup for the meta data for the attribution;
  * 7: The only difference between the dir arr and file arr is that the dir
   has the previous index; which is the parents index;

* ### change_direction
    * 1: Change the tracking direction;
    * 2: In my code, when the user type in, there is a current direction which
  is keep tracking for where the user is;
    * 3: change the current direction to the user prefer direction then go inside 
    * that direction;
* ### delete_directin
    * 1: Doing same as the file; which reset the attribution 
    * 2: Set the datat in the meta block into 0000
* ### print_direction
    * 1: Using the for loop to print out the data which user type in;
    * 2: Checking everysingle array if it contains the datas;

```
int begin(void);
```
**set up all function in order to process**

### **----- NEW ELEMENT FOR META BLOCK FEATURE-----**
```
int updating_metablock(struct Direction *dirlist[], struct File *filelist[]);

int write_metablock_special(int block_index,int special, char item[]);

int loading(struct Direction *dirtable[], struct File *filetable[]);
```
### **----- NEW ELEMENT FOR META BLOCK-----**
* Every single time the user quit the program, the mata block will record all the data inside of the every array of struct; which make it to read when next time the user open;

* write_metablock_special:for location the certain position of the write pointer in the disk; 
the formular i have for my self is position * size_of_each_block + certain_index;

* loading: store the value in the disk when the user open help the programm continuing writing;

###  **---------------- Function declares Finished---------------------------**