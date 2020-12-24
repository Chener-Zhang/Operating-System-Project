# File System Design 
* ## Initializing the size
* ## Partition a disk 
* ## set the main block 
* ## Flat table design 
* ## Regular Funtion

Image :[0000000000000000000000000000000000000000]

# Initializing the size 

[* we can change the number if we want ]\
The **BLOCK_SIZE** is already provide by the sample code which is **4096**\
The **DISK_BLOCKS** is **16384**.\
The total size of the disk is BLOCK_SIZE * DISK_BLOCKS = 4096 x 16384 =64MB
# Partition a disk 
### **The function is already provide by the sample code**
```
int make_disk(char *name){
    make_disk();
}
.......
disk.h
```

# set the main block 

### 1 : Open the disk
```
    Disk_open(); Done!
```
### 2: Set the three main cluster
    * Description of the disk
    * File Imformation + File Pointer to the data 
    * Data 
```
*  Init three main cluster 
    * Description of the disk
    * Before
    * ### Image :[0000000000000000000000000000000000000]
    * After init_description;
    * ### Image :[1111000000000000000000000000000000000]

```

```
 void init_description(size, block,pointer_position){
     
    // define as 0; very very beginning of the disk

     disk_size = size;
     disk_block_number = block;

    //The pointer of this funciton is end after disk_block_number is assigned;     
 }
 ```
### 3: **FAT** Allocation data structure; 


# Flat table design
```
Free space indicatior

struct{

    int[] list = [0,0,0,0,0,0,0,.....];
    // if 0,nothing inside, if 1, something inside

}
```
* If the current block contain something - > list[i] = 1;
* If the current block contain nothing - > list[i] = 0;


```
* VERY IMPORTANT!!!
Direction Entry table;

struct{
    char name[];
    int next_block_pointer_list[];

    sudo:
    Inside the list{
        .. -> previous directory
        . -> current directory
        File1 -> just a file, the list store address
        File2 -> just a file, the list store address        
        New_Direction1 -> new dir, store the ad of struct
        New_Direction2 -> new dir, store the ad of struct
    }

    //if needed
    char other_imformation[];
}
```

```
 Once finished setup the file entery ;
    Before
    Image :[1111000000000000000000000000000000000]
    After Finish the file entry, i mean, it never gonna finish becasue we can increases the block of the entry list;
    Image :[1111001111111111110000000000000000000]

In order to make easy to find the specific entry; 
 we can use the **mmap()** here;
```

# Regular Funtion AND Global Var
```
struct:
    1: Free space indicatior
    2: File entry table
  * 3: Each File descriptor content {
        .
        ..
        file1
        file2
        bla
        bla
        bla
        .
        .
        .
    }
function:    
    1: Make_Disk()
    2: Write_Block()
    3: Read_Block()
    4: Delete_Block_imformation()
    5: Close()
    6: mmap();

```

```
After finished all above
    Before
    Image :[1111001111111111110000000000000000000]
    After put the data in the disk
    Image :[1111001111111111110001111111111110000]

```



```
DISK0 00BLOCK _SIZE000

0000F ILE10FO LDER1000

00000 0000000 00000000

00000 00000FO LDER2000

00000 0.....0 00000000

0000x 0000000 00000000

0000x 000HELL O\0WORLD

0000T HIS IS. ME000000

0000x 3000000 00000000

00000 00012300 0000000

Simulation graph;
in the real case, each block has a remain area for pointer
```

  



# Step
*   CREATE THE DISK
*   INIT THE DISK
*   INIT THE BLOCK
*   WRITE THE BLOCK (int BLOCK_ID, char *text)   
*   READ THE BLOCK(int BLOCK_ID, char *text)   
*   CLOSE()
*   REMEMBER EACH TIME WRITE A NEW BLOCK INCREASE THE SIZE TO KEEP TRACH WHETHER THE BLOCK ARE FULL OR NOT 
*   DONE 


### The project it self can be done by read and write; read and write will be the function that use most in this project



  




 
