#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>
#include "queue.c"


void* function(void * input);


pthread_cond_t producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int limit = 10;
int size_counter = 0;



struct item {
    char* word;
    int threadID;
};



int main(){
    printf("\n\n\n\n");
    printf("hello from the sleep.c\n");
    pthread_t thread = NULL;
    
    for (int i = 0; i < 100; i++) {
        struct item *thread_1 = (struct item *)malloc(sizeof(struct item));
        thread_1->threadID = i;
        pthread_create(&thread,NULL,function,(void*)thread_1);
    }
    
    pthread_join(thread, NULL);
    display();
    printf("\n\n\n\n");
    return 0;
}

void* function(void * input){
    
    
    int number = ((struct item*)input)->threadID;
    printf("age: %d\n",number);

    
    //producer
    pthread_mutex_lock(&lock);
                    
    
    if(size_counter == limit){
        //testing display in the middle
        //display();
        printf("Thread %d go to sleep  --------------------- \n",number);
        pthread_cond_wait(&producer, &lock);
        
    }else{
        enqueue(number,"hello");
        size_counter++;
    }
    
    pthread_cond_broadcast(&consumer);
    pthread_mutex_unlock(&lock);
    
    
    
    //consumer
    pthread_mutex_lock(&lock);
    
    
    if(size_counter == 0){
        //display();
        printf("Thread %d go to sleep  --------------------- \n",number);
        pthread_cond_wait(&consumer, &lock);
        
    }else{
        if(number%2 == 0){
            printf("Thread %d This is an even\n",number);
        }else{
            printf("Thread %d This is an odd\n",number);
        }
        dequeue();
        size_counter--;
    }
    
    pthread_cond_broadcast(&producer);
    
    
    pthread_mutex_unlock(&lock);
    
    
    return 0;
}







