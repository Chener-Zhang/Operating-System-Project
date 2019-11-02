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
        
        perror("Bind Error\n");
        return 1;
    }
    puts("bind done");
     
    //------------------------------------------------------INITIALIZING_FINISH-----------------------------------------------------------
    
    //------------------------------------------------------LISENINGIN_ITIALIZING---------------------------------------------------------
    listen(server_socket , 10);
    puts("waiting for the connection...............\n");
    c = sizeof(struct sockaddr_in);
    
    //------------------------------------------------------LISENING_ING------------------------------------------------------------------

    

    //----------------------------------------------------------THREAD--------------------------------------------------------------------
    
    pthread_t thread_ID;
    while( (client_sock = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c))){
        puts(" Connection Success\n");
        
        
        //Thread creation here;
        struct item *thread_item = (struct item *)malloc(sizeof(struct item));
        thread_item->threadID = client_sock;
        
        if( pthread_create(&thread_ID , NULL ,  function , (void*)thread_item) < 0)
        {
            
            perror("could not create thread");
            return 1;
        }
        
        
        
        
    }
    
    if (client_sock < 0)
    {
        perror("Error something happen!\n ");
        return 0;
    }
    
    
    
    //----------------------------------------------------------THREAD--------------------------------------------------------------------

    
    
    return 0;
}

void* function(void * input){
    
    int client_id_number_in_function = ((struct item*)input)->threadID;
    printf("The client id: %d\n",client_id_number_in_function);
    
    
    //Message creator;
    int buffer_size;
    char *server_message , client_message[2000];
    char *byebye_message = "\n byebye\n";
    char *println_line = ">";
    server_message = "Successful connect to the function\n Please type the world you would like to check, press 'q' for quit\n";
    
    //The first words to the clients;
    send(client_id_number_in_function , server_message , strlen(server_message),0);
    send(client_id_number_in_function , println_line , strlen(println_line),0);
    
    
//----------------------------------------------------------RECEIVING--------------------------------------------------------------------

    while((buffer_size = recv(client_id_number_in_function , client_message , 2000 , 0)) > 0 )
    {
        // size init
        
        printf("size[%d]\n",buffer_size);
        client_message[buffer_size-2] = '\0';
                                
                                //-----EXIT_CHECKER-------------------------------------------------------------------------------------
                                if(client_message[0]=='q'){
                                    send(client_id_number_in_function,byebye_message,strlen(byebye_message),0);
                                    //declare someone is leaving
                                    close(client_id_number_in_function);
                                    break;
                                }else if(buffer_size == -1){
                                    perror("recv failed");
                                    
                                }
                                //-----EXIT_CHECKER-------------------------------------------------------------------------------------
        
        printf("Client %d enter [%s]\n",client_id_number_in_function,client_message);
        
        //------------------------------------- Producer and Consumer ----------------------------------
        
    
        //Producer ---------->
        pthread_mutex_lock(&lock);
        if(size_counter == limit){
              printf("Thread %d go to sleep  --------------------- \n",client_id_number_in_function);
              pthread_cond_wait(&producer, &lock);
        }else{
              enqueue(client_id_number_in_function,client_message);
              size_counter++;
        }
          
          pthread_cond_signal(&consumer);
          pthread_mutex_unlock(&lock);
        
        //consumer ---------->
        pthread_mutex_lock(&lock);
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
            
            //send to the client;
            char* correct_message = "yes\n";
            char* false_message = "no\n";
            //yes / no message
            

               while (fgets(str, MAXCHAR, fp) != NULL){
            
                   int len =strlen(str);
                   str[len-1] = '\0';
                   
                   if(strcmp(client_message,str) == 0){
                       send(client_id_number_in_function , correct_message , strlen(correct_message),0);
                       break;
                   }else{
                       //Do something else;
                   }
               }
            
            // close the file descriptor
            fclose(fp);
            //spell_cheker-------------------------------------------------------------------------------
            dequeue();
            size_counter--;
        }
        
        pthread_cond_signal(&producer);
        pthread_mutex_unlock(&lock);
        
        //------------------------------------- Producer and Consumer ----------------------------------
        
        //clear Ms_buffer
        memset(client_message, 0, 2000);
        send(client_id_number_in_function , println_line , strlen(println_line),0);        
    }
            return 0;

  }







