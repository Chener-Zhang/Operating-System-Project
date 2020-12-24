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


## LAST : My personal perspective to this project; 
* The concurrency is actually very rare too happen; when i do the thread testing, if the number of threads < 100, thread itself is really rare to go to sleep. The reason here is because CPU processing is too fast, if i increase the number of thread to 1000, Then the concurrency is happen; 
  
* What i said above is just testing. so that i can just create more than > 1000 thread. However, during the real clients connect to the server. i create around 5 client or my computer will go very slow. (by the way it impossible for me to open more than 200 terminal console on my laptop, my laptop gonna crash);

* The concurrency will happen in the real life when multi-clients connected to on server for some websize company; Or, in  the school 


* by the way, The queue i import from other  credit goes to him -> Rafael Domingues Tardivo;
