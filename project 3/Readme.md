# Project 3  -   The spell checker 

## There are three major part about this project





* The server connection 
* Multithread - mutual exclusive 
* Spell checker
 
 
 ## 1: The server connection 

* Initilize the socket; 
* Put all init vars into the correct place
* Make the socket 
* Bind the socket 
* Listening
* Accept
            
        
        
 ## 2: Multithread - mutual exclusive 


* Pull the parameter from the function
* Create the ; Actually thread is already create in the main function;
* Set the lock;
* set the condition variables;
* lock and unlock;
* wait and signal;
* reset the buffer;
### EXP Below:
```
lock

producer: 
    Queue FULL yes / no?

    if yes - > go to sleep
    if no - > just keep adding
    wake up consumer

unlock    
------------------------------------------------
lock

consumer
    Queue Empty yes / no?        
    if yes - > go to sleep
    if no -> just keep deleting
    wakeup producer
    
unlock    
```
      
## 3: Spell checker

* Open the file 

        ```
        Using the while loo{
          if (word in dictionary  == yes){
              p(yes)
          }else{ // if no
              p(no)
          } 
        }
        ```
        close the file
        
## 4: Log_file
* Create a buffer
* Every time there is an output of client or server, println, the buffer will goes into the file
* Open + Close file


