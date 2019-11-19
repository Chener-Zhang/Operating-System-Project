#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/mman.h>

int string_spilit(int start, int end, char letter[]);


char words[50];
int main(){

    int block_size = 5;

    printf("\n\n");
    char user_input[50];    
    printf(">"); // print ">";
    fgets(user_input,sizeof(user_input),stdin); // get user_input;    

    int user_input_len = (int)strlen(user_input);            
    printf("The length of the letter is %d\n",user_input_len);    

    int blocks_need = user_input_len / block_size;
    int remainder = user_input_len % block_size;

    printf("needs %d of blocks \n",blocks_need);
    printf("remainder is  %d \n",remainder);
    


    //block write;

    memset(words,0,sizeof(words));

    for (int i = 0; i < block_size; i++)
    {
        printf("%c",user_input[i]);
        

    }
    printf("\n");

    int end_index_catcher;

    for (int i = 1; i < blocks_need - 1 ; i++)
    {
            int index_start = i * block_size;
            int index_end = index_start + block_size;
            printf("start index %d  end index %d\n",index_start,index_end);
            string_spilit(index_start,index_end,user_input);
            end_index_catcher = index_end;
    }
    

    for (int i = end_index_catcher; i < user_input_len; i++)
    {
        printf("%c",user_input[i]);

    }
    

    printf("\n");
    return 0;
}


int string_spilit(int start, int end, char letter[]){
    for (int i = start; i < end; i++){
        printf("%c",letter[i]);
    }
    printf("\n");
    return 0;
}