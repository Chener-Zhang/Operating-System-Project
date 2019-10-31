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

int x = 0;

int limit = 10;
int size_counter = 0;



struct item {
    char* name;
    int age;
};



int main(){
    printf("\n\n\n\n");
    printf("hello from the sleep.c\n");
    pthread_t thread = NULL;
    
    for (int i = 0; i < 10; i++) {
        struct item *Allen = (struct item *)malloc(sizeof(struct item));
        
        Allen->age = i;
        pthread_create(&thread,NULL,function,(void*)Allen);
    }
    
    pthread_join(thread, NULL);
    printf("\n\n\n\n");
    return 0;
}

void* function(void * input){

    int number = ((struct item*)input)->age;
    printf("age: %d\n",number);

    
    //producer
    pthread_mutex_lock(&lock);
                    
    
    if(size_counter == limit){
        printf("------zzzzzz------\n");
        pthread_cond_wait(&producer, &lock);
        
    }else{
        size_counter++;
    }
    
    pthread_cond_signal(&consumer);
    pthread_mutex_unlock(&lock);
    
    
    //consumer
    pthread_mutex_lock(&lock);
    if(size_counter == 0){
        printf("------zzzzzz------\n");
        pthread_cond_wait(&consumer, &lock);
        
    }else{
        if(number%2 == 0){
            printf("Number %d This is an even\n",number);
        }else{
            printf("Number %d This is not an even\n",number);
        }
        
        size_counter--;
    }
    pthread_cond_signal(&producer);
    
    
    pthread_mutex_unlock(&lock);
    
    return 0;
}







