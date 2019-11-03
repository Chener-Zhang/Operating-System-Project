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


#define MAXCHAR 50

//The void* function;
void* function(void * input);

//Vars;
pthread_cond_t producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


// Global vars
int limit = 10;
int size_counter = 0;

//my_struct which carry the user_input and the id;
struct item {
    char* word;
    int threadID;
};

// run;
int main(){
    
    //------------------------------------------------------INITIALIZING------------------------------------------------------------------
    int server_socket , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (server_socket == -1)
    {
        printf("Error!\n");
    }
    puts("Socket creation success\n");
     
    //Initializing
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888); // My port number;
     
    //Bind togehter
    if( bind(server_socket,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
    //If less than 0, then something happen badly
        perror("Bind Error\n");
        return 1;
    }
    puts("bind done");
     
    //------------------------------------------------------INITIALIZING_FINISH-----------------------------------------------------------
    
    //------------------------------------------------------LISENINGIN_ITIALIZING---------------------------------------------------------
    // wait for the lient enters
    listen(server_socket , 10);
    puts("waiting for the connection...............\n");
    c = sizeof(struct sockaddr_in);
    
    //------------------------------------------------------LISENING_ING------------------------------------------------------------------

    

    //----------------------------------------------------------THREAD--------------------------------------------------------------------
    // thread creation;
    // If anyone enters in, it will create the thread;
    pthread_t thread_ID;
    while( (client_sock = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c))){
        puts(" Connection Success\n");
        
        // initialize the struct item;
        struct item *thread_item = (struct item *)malloc(sizeof(struct item));
        thread_item->threadID = client_sock;
        
        
        //Thread creation here;
        if( pthread_create(&thread_ID , NULL ,  function , (void*)thread_item) < 0)
        {
        // if < 0, the creation of thread is fail;
            perror("could not create thread");
            return 1;
        }
        
    }
    
    // same here that the client has some problem with the connection;
    if (client_sock < 0)
    {
        perror("Error something happen!\n ");
        return 0;
    }
    
    
    
    //----------------------------------------------------------THREAD--------------------------------------------------------------------

    
    
    return 0;
}

void* function(void * input){
    // set input to client_id_number_in_function, casting the variable;
    int client_id_number_in_function = ((struct item*)input)->threadID;
    printf("The client id is enter the room: %d\n",client_id_number_in_function);
    
    
    //<-----Message creator--------->
    int buffer_size;
    // size
    char *server_message , client_message[2000];
    // byebye text
    char *byebye_message = "\n byebye\n";
    // prompt
    char *println_line = ">";
    // press q to quit
    server_message = "Successful connect to the function\n Please type the world you would like to check, press 'q' for quit\n";
    
    //The first words to the clients;
    // send something to my clients
    send(client_id_number_in_function , server_message , strlen(server_message),0);
    send(client_id_number_in_function , println_line , strlen(println_line),0);
    
    
//----------------------------------------------------------RECEIVING--------------------------------------------------------------------
    //If receive;
    while((buffer_size = recv(client_id_number_in_function , client_message , 2000 , 0)) > 0 )
    {
        
        client_message[buffer_size-2] = '\0';
        
                                //-----EXIT_CHECKER-------------------------------------------------------------------------------------
                                if(client_message[0]=='q'){
                                    send(client_id_number_in_function,byebye_message,strlen(byebye_message),0);
                                    //declare someone is leaving
                                    printf("Client[%d] is leaving............\n",client_id_number_in_function);
                                    close(client_id_number_in_function);
                                    break;
                                }else if(buffer_size == -1){
                                    perror("recv failed");
                                    
                                }
                                //-----EXIT_CHECKER-------------------------------------------------------------------------------------
        
        
        
        printf("The client [%d] enters [%s]\n",client_id_number_in_function,client_message);
        //------------------------------------- Producer and Consumer Algorithem ----------------------------------
        
    
        //Producer -------lock first because i need to use wait and signal, make sure mutex is here. no thread come here more than 2--->
        pthread_mutex_lock(&lock);
        if(size_counter == limit){
            // if oversize
              printf("Thread %d go to sleep  --------------------- \n",client_id_number_in_function);
              pthread_cond_wait(&producer, &lock);
        }else{
            //if not oversize just come in
              enqueue(client_id_number_in_function,client_message);
              size_counter++;
        }
            // unclock and wakeup the consumer
          pthread_cond_signal(&consumer);
          pthread_mutex_unlock(&lock);
        
        //consumer ---------->
        pthread_mutex_lock(&lock);
        // if nothing in the queue
        if(size_counter == 0){
            printf("Thread %d go to sleep  --------------------- \n",client_id_number_in_function);
            pthread_cond_wait(&consumer, &lock);
            
        }else{
            //spell_cheker-------------------------------------------------------------------------------
            // define a file
            FILE *fp;
            // define the str
            char str[MAXCHAR];
            // address the file name
            char* filename = "dictionary.txt";
            //got the file descriptor number;
            fp = fopen(filename, "r");
            //if something wired happen;
            if (fp == NULL){
                printf("Could not open file %s",filename);
                return (void*)1;
            }
                        
            //assign buffer_size;
            size_t bufsize = 32;
            
            int input_len =strlen(str);
            // false_couter is set it 0, there are 99171 word inside of the dictionary, if the word is not in the dictionary it means
            // the checker have already check everything inside; then it will print no;
            int false_counter = 0;
            //send to the client;
            char* correct_message = "The word is in the dictionary\n";
            char* false_message = "The word is NOT in the dictionary\n Error\n";
            //yes / no message
            
            // check whether the words enter by the user is actually inside of the dictionary
               while (fgets(str, MAXCHAR, fp) != NULL){
            
                   int len =strlen(str);
                   str[len-1] = '\0';
                   // if yes;
                   if(strcmp(client_message,str) == 0){
                       send(client_id_number_in_function , correct_message , strlen(false_message),0);
                       break;
                   }else{
                       //else no
                       false_counter++;
                       if(false_counter == 99171){
                       send(client_id_number_in_function , false_message , strlen(false_message),0);
                       }
                                              
                   }
               }
            
            // close the file descriptor
            fclose(fp);
            //spell_cheker-------------------------------------------------------------------------------
            // take out the stuff;
            dequeue();
            size_counter--;
        }
        // wake up the producer,
        pthread_cond_signal(&producer);
        // unlock this area
        pthread_mutex_unlock(&lock);
        
        //------------------------------------- Producer and Consumer ----------------------------------
        
        //clear Ms_buffer
        memset(client_message, 0, 2000);
        send(client_id_number_in_function , println_line , strlen(println_line),0);
    }
            return 0;

  }







