#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include<time.h>


///////////////INITIALIZER///////////////////////
int globle_id;
int globle_type;
int globle_time;
int current_time;
int final_time;
int arrive_min;
int arrive_max;
int cpu_min;
int cpu_max;
int disk1_min;
int disk2_min;
int dist1_max;
int dist2_max;
///////////////QUEUE_SIZE_COUNTER///////////////////////
double average_cpu_queue_size = 0;//CPU QUEUE
int max_cpu_queue_size = 0;

double average_disk1_queue_size = 0;//DISK1 QUEUE
int max_disk1_queue_size = 0;
double average_disk2_queue_size = 0;//DISK2 QUEUE
int max_disk2_queue_size = 0;

///////////////TIME_UTILITIE///////////////////////
double CPU_time_utilities = 0;
double DISK1_time_utilities = 0;
double DISK2_time_utilities = 0;

///////////////RESPONSE TIME ///////////////////////
int cpu_max_response_time;
double cpu_average_response_time;
int DISK1_max_response_time;
double DISK1_average_response_time;
int DISK2_max_response_time;
double DISK2_average_response_time;

///////////////THROUGHPUT Time ///////////////////////
double numbers_of_job_in_cpu;
double numbers_of_job_in_disk1;
double numbers_of_job_in_disk2;

// /////////////////////////////////////////////////

//A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

//A structure to CREATE A QUEUE
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

// ASSIGNMENT NUMBER FROM CONFIG TO THE INITIALIZER
void assign_var(){
    FILE* file = fopen("/Users/chenerzhang/Desktop/hw1/config.txt", "r");
    int arr[10];
    for (int i = 0; i < 10; i++) {
        int number;
        char item[20];
        fscanf(file,"%s",item);
        // printf("%s\n",item);
        fscanf(file,"%i",&number);
        //printf("%d\n",number);
        arr[i] = number;
        //printf("\n");
    }
    fclose(file);
    
    current_time = arr[0];
    final_time = arr[1];
    arrive_min = arr[2];
    arrive_max = arr[3];
    cpu_min = arr[4];
    cpu_max  = arr[5];
    disk1_min  = arr[6];
    disk2_min  = arr[7];
    dist1_max  = arr[8];
    dist2_max  = arr[9];
    
    for (int i = 0; i<10; i++) {
        //printf("[%d]",arr[i]);
    }
    
    /////[TESTING]/////
    //printf("\n");
    /////[TESTING]/////
}

//CHECK QUEUE IS FULL
int isFull(struct Queue* queue)
{ return (queue->size == queue->capacity); }

//CHECK QUEUE IS EMPTY
int isEmpty(struct Queue* queue)
{ return (queue->size == 0); }

//ENQUEUE
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item);
}

//DEQUEUE
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// TAKE THE FRONT ELEMENT
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}


//////HEAP//////////HEAP/////////HEAP////////HEAP//////////HEAP////////HEAP////////HEAP////////////
struct Event {
    int R_number; // R_number is actually the ramdom number that i will assign in the var later
    int event_type; // case number
    int ID_Number; //process id
};
// heap structure
struct Heap
{
    int capacity;
    int size;
    struct Event* List[];
};

struct Heap* create_min_heap(int capa){
    struct Heap* heap = (struct Heap*) malloc(sizeof(struct Heap));
    heap->capacity = capa;
    heap->size = 0;
    return heap;
};

int getParent(int index){
    double n = (index - 1)/2;
    int number = floor(n);
    return number;
}


void siftup(int eventindex, struct Heap* heap){
    int parentindex;
    struct Event* temp;
    if(eventindex!=0){
        parentindex = getParent(eventindex);
        if(heap->List[parentindex]->R_number > heap->List[eventindex]->R_number ){
            temp = heap->List[parentindex];
            heap->List[parentindex] = heap->List[eventindex];
            heap->List[eventindex] = temp;
            siftup(parentindex,heap);
        }
    }
    
}

void insertion(struct Heap* heap,int type, int RG_N,int id_number){
    struct Event* event = (struct Event*) malloc(sizeof(struct Event));
    if(heap->size == heap->capacity){
        printf("The heap is full");
    }else{
        
        heap->size++;
        int c = heap->size - 1 ;
        event->R_number = RG_N;
        event->event_type = type;
        event->ID_Number = id_number;
        heap->List[c] = event;
        siftup(heap->size - 1,heap);
    }
    
}


void heapify_top_bottom(struct Heap* heap, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    struct Event* temp;
    
    if(left >= heap->size || left <0)
        left = -1;
    if(right >= heap->size || right <0)
        right = -1;
    
    if(left != -1 && heap->List[left]->R_number < heap->List[parent_node]->R_number)
        min=left;
    else
        min =parent_node;
    if(right != -1 && heap->List[right]->R_number < heap->List[min]->R_number)
        min = right;
    
    if(min != parent_node){
        temp = heap->List[min];
        heap->List[min] = heap->List[parent_node];
        heap->List[parent_node] = temp;
        
        // recursive  call
        heapify_top_bottom(heap, min);
    }
    
}

int PopMin(struct Heap* heap){
    int pop;
    if(heap->size==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    pop = heap->List[0]->ID_Number;
    globle_id = heap->List[0]->ID_Number;
    globle_type = heap->List[0]->event_type;
    globle_time = heap->List[0]->R_number;
    heap->List[0] = heap->List[heap->size-1];
    heap->size--;
    heapify_top_bottom(heap, 0);
    return pop;
}


void print_heap(struct Heap* heap){
    int len = heap->size;
    for (int i = 0; i < len; i ++) {
        printf("R_Number:[%d] ID:[%d]  Type:[%d]  ",heap->List[i]->R_number,heap->List[i]->ID_Number,heap->List[i]->event_type);
        printf("\n");
    }
    
    
}
//////HEAP//////////HEAP/////////HEAP////////HEAP//////////HEAP////////HEAP////////HEAP////////////


//create the R_number;
//min and max take the parameter from the initializer
int random_number_generator(int min, int max){
    
    int n = rand() % (max + 1 - min) + min;
    return n;
}

//find the max number in the queue
int find_max_queue(struct Queue* list){
    int max =list->array[0];
    for (int i = 1; i < list->size; i++) {
        if(list->array[i] > max){
            max = list->array[i];
        }
    }
    //printf("The max is %d\n",max);
    return max;
}

//calculate the average size of each queue
double average_size_calculator(struct Queue* list){
    double sum = 0;
    for (int i  = 0 ; i < list->size; i++) {
        sum+=list->array[i];
    }
    //testing the change of sum
    //printf("the sum is %d the size is %d\n",sum, list->size);
    sum = sum/list->size;
    //printf("this is the %d\n",sum);
    return sum;
}

//the simulation begin//
void begin(struct Heap* heap){
    //1: assign the variable, initialize the variable
    assign_var();
    //2: insert the first heap, becasue the heap cannot be empty or the while loop will be stop
    insertion(heap, 1, 0, 1);
    //3: ready for the log.txt because i will print the outcome in to the log file
    FILE* log = fopen("/Users/chenerzhang/Desktop/hw1/log.txt", "w");
    
    //4: just the formating stuff reminds me.
    //insertion(<#struct Heap *heap#>, <#int type#>, <#int RG_N#>, <#int id_number#>)
    
    
    //5: initiliza the status of whether busy or not
    // yes busy -> 1
    // no busy -> 0
    
    int CPU_status = 0;
    int Disk1_status = 0;
    int Disk2_status = 0;
    int job_number = 1;
    // at least 1 job inside the queue the job i just create
    //initialize the Queue; i do 10000 just in case it full;
    struct Queue* cpu_queue = createQueue(10000);
    struct Queue* disk1_queue = createQueue(10000);
    struct Queue* disk2_queue = createQueue(10000);
    //////////////////////////////////////////////////////
    struct Queue* cpu_size_counter = createQueue(10000);
    struct Queue* disk1_size_counter = createQueue(10000);
    struct Queue* disk2_size_counter = createQueue(10000);
    //////////////////////////////////////////////////////
    struct Queue* cpu_response = createQueue(10000);
    struct Queue* disk1_response = createQueue(10000);
    struct Queue* disk2_response = createQueue(10000);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //ramdom number generator
    srand((int) time(NULL));
    // while loop begin --->
    
    while((heap->size > 0) && globle_time < final_time)
    {
        /* THIS IS JUST FOR ME TO TESTING, DEBUG, AND TRACKING THE HEAP
         //printf("-----------------------\n");
         //print_heap(heap);
         //printf("-----------------------\n");
         */
        
        //Take out the first element
        
        PopMin(heap);
        int x = globle_type; // x = case / define the type of case
        
        /*
         printf("log: %d\n",globle_time);
         printf("current is %d\n",globle_id);
         printf("-----------------------\n");
         printf("the size_of_cpu_is %d\n",cpu_queue->size);
         printf("the size_of_disk1_is %d\n",disk1_queue->size);
         printf("the size_of_disk2_is %d\n",disk2_queue->size);
         printf("-----------------------\n");
         printf("Status: \n");
         */
        
        // put the correct type into correct case //
        switch (x) {
                //--------------------------------------------------------------------------------------------------//
            case 1: // Process arrives
            {
                //printf("At time %d : process %d enter system\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d enter system\n",globle_time,globle_id);
                if(CPU_status){/// is busy
                    enqueue(cpu_queue, globle_id);
                    // printf("At time %d : process %d enter CPU queue\n",globle_time,globle_id);
                    fprintf(log, "At time %d : process %d enter CPU queue\n",globle_time,globle_id);
                }else{//// is not busy
                    if(cpu_queue->size == 0){
                        CPU_status  = 1 ;
                        insertion(heap, 2, globle_time, globle_id);
                        //   printf("At time %d : process %d enter CPU\n",globle_time,globle_id);
                        fprintf(log, "At time %d : process %d enter CPU\n",globle_time,globle_id);
                    }else{// if size > 0
                        CPU_status  = 1;
                        // printf("At time %d : process %d go to the CPU queue\n",globle_time,globle_id);
                        fprintf(log, "At time %d : process %d go to the CPU queue\n",globle_time,globle_id);
                        globle_id = dequeue(cpu_queue);
                        insertion(heap, 2, globle_time, globle_id);
                        //printf("At time %d : process %d enter CPU\n",globle_time,globle_id);
                        fprintf(log, "At time %d : process %d enter CPU\n",globle_time,globle_id);
                    }
                }
                job_number++;
                int arr = random_number_generator(arrive_min, arrive_max) + globle_time;
                insertion(heap, 1, arr, job_number);
                break;
            }
                
                
                //--------------------------------------------------------------------------------------------------//
            case 2://CPU begins
            {
                // assign the number; insert into the queue;
                numbers_of_job_in_cpu ++;
                int time_process = random_number_generator(cpu_min, cpu_max);
                CPU_time_utilities += time_process;
                enqueue(cpu_response, time_process);
                int ramdon_number = time_process+ globle_time;
                insertion(heap, 3, ramdon_number, globle_id);
                //printf("At time %d : process %d CPU process begin\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d CPU process begin\n",globle_time,globle_id);
                globle_time = ramdon_number;
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 3://Finished at CPU, so that the status become 0 again;
            {
                CPU_status = 0;
                //printf("At time %d : process %d is finished at CPU \n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d is finished in CPU \n",globle_time,globle_id);
                int probability_out = random_number_generator(2, 10);
                
                //When the cpu finished ------ here is the probability to determind it continue or out //
                
                //TESTING
                //printf("the probability of out is %d\n",probability_out);
                if(probability_out <= 2){
                    // printf("At time %d : process %d is leave system\n",globle_time,globle_id);
                    fprintf(log, "At time %d : process %d is leave system\n",globle_time,globle_id);
                    insertion(heap, 4, globle_time, globle_id);
                }else //Continue to the Disk Event:
                {
                    //////////Disk1 check busy/////////
                    if(Disk1_status)///Disk 1 -> yes busy
                    {
                        /////checkBusy(Disk2)///////
                        switch (Disk2_status) {
                            case 0:{///// no busy0
                                enqueue(disk2_queue, globle_id);
                                globle_id = dequeue(disk2_queue);
                                insertion(heap, 6, globle_time, globle_id);
                                //printf("At time %d : process %d arrives at the disk 2\n",globle_time,globle_id);
                                fprintf(log, "At time %d : process %d arrives at the disk 2\n",globle_time,globle_id);
                                Disk2_status = 1;
                                break;
                            }
                            default:///yes busy 1
                            {
                                
                                //////compare size//////
                                if(disk1_queue->size > disk2_queue->size){
                                    enqueue(disk2_queue, globle_id);
                                }else if (disk1_queue->size < disk2_queue->size){
                                    enqueue(disk1_queue, globle_id);
                                }else{//// d1.size = d2.size
                                    enqueue(disk1_queue, globle_id);
                                }
                                break;
                            }
                        }
                        /////checkBusy(Disk2)///////
                    }else//Disk 1 -> no busy
                    {
                        enqueue(disk1_queue, globle_id);
                        globle_id = dequeue(disk1_queue);
                        insertion(heap, 5, globle_time, globle_id);
                        Disk1_status = 1;
                        //printf("At time %d : process %d arrives at disk 1\n",globle_time,globle_id);
                        fprintf(log, "At time %d : process %d arrives at disk 1\n",globle_time,globle_id);
                    }
                    //////////Disk1 check busy/////////
                }
                ///////////////////PROBABILITY END//////////////////////////
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 4: // PROCESS OUT
            {
                //printf("At time %d : process %d exist system\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d exist system\n",globle_time,globle_id);
                break;
                
            }
                //--------------------------------------------------------------------------------------------------//
            case 5: //PROCESS GO TO DISK1
            {
                numbers_of_job_in_disk1++;
                int time_process = random_number_generator(disk1_min, dist1_max);
                enqueue(disk1_response, time_process);
                
                int random_time = time_process + globle_time;
                DISK1_time_utilities += time_process;
                
                insertion(heap, 7, random_time, globle_id);
                //printf("At time %d : process %d begins I/O on Disk 1.\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d begins I/O on Disk 1.\n",globle_time,globle_id);
                globle_time = random_time;
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 6: //PROCESS GO TO DISK2
            {
                numbers_of_job_in_disk2++;
                int time_process = random_number_generator(disk2_min, dist2_max);
                enqueue(disk2_response, time_process);
                int random_time = time_process + globle_time;
                DISK2_time_utilities += time_process;
                insertion(heap, 8, random_time, globle_id);
                //printf("At time %d : process %d begins I/O on Disk 2.\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d begins I/O on Disk 2.\n",globle_time,globle_id);
                globle_time = random_time;
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 7:// FINISHED DISK 1
            {
                Disk1_status = 0;
                insertion(heap, 9, globle_time, globle_id);
                //printf("At time %d : process %d IS finish at disk 1 \n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d IS finish at disk 1 \n",globle_time,globle_id);
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 8://FINISHED DISK 2
            {
                Disk2_status = 0;
                insertion(heap, 9, globle_time, globle_id);
                //printf("At time %d : process %d finish at disk 2\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d finish at disk 2\n",globle_time,globle_id);
                break;
            }
                //--------------------------------------------------------------------------------------------------//
            case 9:// ALL SET
            {
                //printf("At time %d : process %d is out of system\n",globle_time,globle_id);
                fprintf(log, "At time %d : process %d is out of system\n",globle_time,globle_id);
                break;
            }
                
        }
        //--------------------------------------------------------------------------------------------------//
        // THIS IS FOR THE HW PRESENTATION FOR STAT.TXT
        enqueue(cpu_size_counter, cpu_queue->size);
        enqueue(disk1_size_counter, disk1_queue->size);
        enqueue(disk2_size_counter, disk2_queue->size);
        
    }//while loop finish
    //--------------------------------------------------------------------------------------------------//
    average_cpu_queue_size = average_size_calculator(cpu_size_counter);
    max_cpu_queue_size = find_max_queue(cpu_size_counter);
    
    average_disk1_queue_size = average_size_calculator(disk1_size_counter);
    max_disk1_queue_size = find_max_queue(disk1_size_counter);
    
    average_disk2_queue_size = average_size_calculator(disk2_size_counter);
    max_disk2_queue_size = find_max_queue(disk2_size_counter);
    //--------------------------------------------------------------------------------------------------//
    
    
    /*
     printf("---------Queues-----\n");
     printf("Average size of CPU Queue: %f\n", average_cpu_queue_size);
     printf("Max size of CPU Queue: %d\n",max_cpu_queue_size);
     printf("Average size of DISK 1 Queue: %f\n", average_disk1_queue_size);
     printf("Max size of CPU Queue: %d\n",max_disk1_queue_size);
     printf("Average size of DISK 2 Queue: %f\n", average_disk2_queue_size);
     printf("Max size of CPU Queue: %d\n",max_disk1_queue_size);
     printf("---------Utilization-----\n");
     printf("The time spend in CPU total is %f\n",CPU_time_utilities/100000);
     printf("The time spend in DISK1 total is %f\n",DISK1_time_utilities/100000);
     printf("The time spend in DISK2 total is %f\n",DISK2_time_utilities/100000);
     printf("---------Response_time-----\n");
     printf("CPU average response time: %f\n",average_size_calculator(cpu_response));
     printf("CPU max response time: %d\n",find_max_queue(cpu_response));
     printf("DISK1 average response time: %f\n",average_size_calculator(disk1_response));
     printf("DISK1 max response time: %d\n",find_max_queue(disk1_response));
     printf("DISK2 average response time: %f\n",average_size_calculator(disk2_response));
     printf("DISK2 max response time: %d\n",find_max_queue(disk2_response));
     printf("---------Throughput-----\n");
     printf("Throughput of CPU:  %f Jobs per Unit of Time\n",numbers_of_job_in_cpu/globle_time);
     printf("Throughput of DISK1:  %f Jobs per Unit of Time\n",numbers_of_job_in_disk1/globle_time);
     printf("Throughput of DISK2:  %f Jobs per Unit of Time\n",numbers_of_job_in_disk2/globle_time);
     */
    //--------------------------------------------------------------------------------------------------//
    FILE* stat = fopen("/Users/chenerzhang/Desktop/hw1/stat.txt", "w");
    fprintf(stat,"---------Queues-----\n");
    fprintf(stat,"Average size of CPU Queue: %f\n", average_cpu_queue_size);
    fprintf(stat, "Max size of CPU Queue: %d\n",max_cpu_queue_size);
    fprintf(stat, "Average size of DISK 1 Queue: %f\n", average_disk1_queue_size);
    fprintf(stat, "Max size of CPU Queue: %d\n",max_disk1_queue_size);
    fprintf(stat, "Average size of DISK 2 Queue: %f\n", average_disk2_queue_size);
    fprintf(stat, "Max size of CPU Queue: %d\n",max_disk1_queue_size);
    fprintf(stat, "---------Utilization-----\n");
    fprintf(stat, "The time spend in CPU total is %f\n",CPU_time_utilities/100000);
    fprintf(stat, "The time spend in DISK1 total is %f\n",DISK1_time_utilities/100000);
    fprintf(stat, "The time spend in DISK2 total is %f\n",DISK2_time_utilities/100000);
    fprintf(stat, "---------Response_time-----\n");
    fprintf(stat, "CPU average response time: %f\n",average_size_calculator(cpu_response));
    fprintf(stat, "CPU max response time: %d\n",find_max_queue(cpu_response));
    fprintf(stat, "DISK1 average response time: %f\n",average_size_calculator(disk1_response));
    fprintf(stat, "DISK1 max response time: %d\n",find_max_queue(disk1_response));
    fprintf(stat,"DISK2 average response time: %f\n",average_size_calculator(disk2_response));
    fprintf(stat, "DISK2 max response time: %d\n",find_max_queue(disk2_response));
    fprintf(stat, "---------Throughput-----\n");
    fprintf(stat, "Throughput of CPU:  %f Jobs per Unit of Time\n",numbers_of_job_in_cpu/globle_time);
    fprintf(stat,"Throughput of DISK1:  %f Jobs per Unit of Time\n",numbers_of_job_in_disk1/globle_time);
    fprintf(stat, "Throughput of DISK2:  %f Jobs per Unit of Time\n",numbers_of_job_in_disk2/globle_time);
    //--------------------------------------------------------------------------------------------------//
    
    //print_heap(heap);
    //printf("-----------------------\n");
    //EMPTY THE HEAP
    while(heap->size>0){
        PopMin(heap);
    }
    
    insertion(heap, 0, 100000, 0);
    //printf("log 100000 : simulation Finish\n");
    int final = PopMin(heap);
    fprintf(log,"log %d : simulation Finish\n",final);
    //CLOSE THE FILE
    fclose(stat);
    fclose(log);
    
    
}



int main(){
    //running the code
    struct Heap* list = create_min_heap(200);
    
    begin(list);
    
    srand(( int) time(NULL));
    free(list);
    printf("Runs finished\n");
    return 0;
}

