# Project Description



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
* ### Read_file
* ### Delete_file

* ****


```
int Create_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir);
int Change_directory(char *dirname,struct Direction *current_dir, struct Direction *dir_table[]);
int Delete_directory(char *dirname, struct Direction *dir_table[], struct Direction *current_dir,struct File *file_table[]);
void print_direction( struct Direction *dir,struct Direction *list[],struct File *file_table[]);
```
### **Direction manipulation Function**
* ### create_direction
* ### change_direction
* ### delete_directin
* ### print_direction


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