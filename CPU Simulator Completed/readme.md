<!-----heading ---->
# 1: A high level overview of your program 

### The goal for my program is to make a spu simulation generator which include the cpu, disk 1 and disk2  

# 2: The data structure i use most is:

### 1: Heap
### 2: Queue
### 3: especially for the Queue. I use it as the regular device line, and size counter. 
### 4: the ramdom number generator

# How did you represent your data?
- using print out method 
- deliver the date to the log.txt file 

# What is the “work-flow” of your program?
- my simulation is very straight forward


- initialize the variable 
- begin 
- pop()
- check(status from case 1 to 9) 
  
####  CASE 1 JOB ARRIVE 
- job arrive 
    - job cpu begin, in side check the cpu status and the size stuff. 
    - whether it is busy or not, 
    - if busy then .... 
    - if not busy then....



 
 ####  CASE 2 CPU BEGIN
- insert the number ramdom number which has the range, and insert into the heap;    

 ####  CASE 3 CPU FINISH
- do the probability generator in order to decide it out or continue to the disk1/2
    -    let set x = 3 (0-10);
    -    x > 3 then .... out? if out jump to case 4
    -    x < 3 then .... continue? do next 
    -    check the d1 check(busy);
    -    if busy then .... 
         -    check disk 2 
         -    busy(disk2)
              -    yes
                   -    yes then go to d2 line 
              -    no
                   -    use d2
    -    if not busy then... 
    -    if both busy, then just go into the line 

 ####  CASE 4 OUT 
- just print out that process x is out 

 ####  CASE 5 DISK 1 BEGIN 
- Generate a new ramdom number which given the range.
  - insert into the heap 
  
 ####  CASE 6 DISK 2 BEGIN 
- same as disk1 CASE 5
 ####  CASE 7 DISK 1 FINISHED 
-  print finish

 ####  CASE 8 DISK 2 FINISHED  
-  printf finish 
 ####  CASE 9 Everything all set

-  #### print finish 



# 
# COMBINATION 
## Combine all the the function together into new function.
run it, and all set. 


# How i feel 
## First, the most challenging part for this assignment is the ramdom number generator part and creation of heap and queue. 
## Second, the case 2 and case 3 is quite hard,Especially case 3, before i start for the case 2, i actually do the layout. [here is the link](https://www.mindmeister.com/1327361311#meistertask)
## If this is not able to open i also attached a new png in my github.


  






  





