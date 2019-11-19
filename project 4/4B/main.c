#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/mman.h>

int string_spilit(int start, int end, char letter[]);

int main(){

    int block_size = 5;

    printf("\n\n");
    char user_input[50];    
    printf(">"); // print ">";
    fgets(user_input,sizeof(user_input),stdin); // get user_input;    

    int user_input_len = (int)strlen(user_input);    
    printf("[%s]\n",user_input);
    printf("The length of the letter is %d\n",user_input_len);    

    int blocks_need = user_input_len / block_size;
    printf("needs %d of blocks \n",blocks_need);
    
    printf("\n\n");
    return 0;
}


int string_spilit(int start, int end, char letter[]){

    return 0;
}