# Hi, Welcome to my project 4

* -> Click on "4A" for "Design of the Simple File System" ---- Project4 - A Psudo code 
* -> Click on "4B" for "Implementing a Simple File System on Top of a Virtual Disk" ---- for my project entire code

# Enjoy
 

**Notes for myself**
## write(); write stuff
```
write(int fd, "words", strlen(words));
```


## fseek(); set where to begin
```
fseek(int fd, index_pointer, SEEK_SET );
//SEEK_SET: set the start pointer here 
exp: 
------------------------
1: fd = open("this is a test")
------------------------
2: before : this is a test 
------------------------
3: fseek(int fd, 4, SEEK_SET );
------------------------
4: after  : is a test
------------------------

```

## mmap(); set where to begin
```


void *addr = mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

-----------------
List :[0 0 t 0 h 0 i 0 s]

Index:[0 1 2 3 4 5 6 7 8] 
-----------------
char *list = addr;

=> will print

list[0] = '0';
list[2] = 't';
list[4] = 'h';


```


```
struct {
    char name[];
    int size;
    int next_entry;
    char time_of_the_creation[]
    .
    .
    .
    .

}File;
```

```
struct {
    char name[];
    int size[]
    ine next_entry;
    .
    .
    .

}Direction 
```
```
struct{
    int index_of_the_block / block_id;
    int size;
    int next_entry;

}BLOCK
```
