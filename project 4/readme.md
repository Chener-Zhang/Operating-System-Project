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