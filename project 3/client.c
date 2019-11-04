#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>

//Define the port number;
#define PORT 8888

int main(int argc, char const *argv[]) 
{
    // init which is same as the server_socket;
    //-------------------------- Initializing ------------------------------------------------------------
    int sock = 0;
	struct sockaddr_in serv_addr;
    
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("Error! \n");
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
		 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("Error! \n");
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{ 
		printf("Connection Failed\n");
		return -1; 
	}
    //-------------------------- Initializing ------------------------------------------------------------
    
    
    
    //-------------------------- Message sending ------------------------------------------------------------

    //Define the size of the server_message;
    int server_message_size;
    char server_message[2000];
    while(1){
                  char user_input[100];
                  printf( "Enter a value :");
                  gets(user_input);
                  int len = (int)strlen(user_input);
                  user_input[len - 2] = '\0';
                  //testing;
                  //printf("%lu\n",strlen(str));
                  //printf("you have enter\n");
                  //printf("%s\n",str);
                            
        
        
        // if we getting something from the server;
        send(sock , user_input , strlen(user_input) , 0 );

        if(server_message_size> 0 ){
            //define the receive;
            server_message_size = recv(sock , server_message , 2000 , 0);
            puts(server_message);
        }
        // send the thing to the server;
    }
    
    //-------------------------- Message sending ------------------------------------------------------------
	return 0; 
} 

