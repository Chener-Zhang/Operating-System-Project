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
    printf("The length of the letter is %d\n",user_input_len);    

    int blocks_need = user_input_len / block_size;
    int remainder = user_input_len % block_size;

    printf("needs %d of blocks \n",blocks_need);
    printf("remainder is  %d \n",remainder);
    


    
    char buffer[50];

    
    
    memcpy(buffer, user_input,block_size-1);
    
    printf("block 1\n");
    printf("[%s]\n",buffer);
    printf("\n");


    printf("block 2\n");
    int end_index_from_loop;
    //memset(buffer,0,sizeof(buffer));
    for (int i = 1; i < blocks_need; i++)
    {            
            int index_start = i * block_size;
            int index_end = index_start + block_size;            
            memcpy(buffer, &user_input[index_start],block_size);
            printf("[%s]\n",buffer);
            end_index_from_loop = index_end;
    }
    if(remainder == 0){
        return 0;
    }else{
            printf("block last\n");
            memcpy(buffer, &user_input[end_index_from_loop],block_size);
            printf("[%s]\n",buffer);
    } 
    printf("\n");
    return 0;
}





/*
//something need to happen;                                            
            int blocks_need = user_input_len/each_block_size;
            int remainder = user_input_len % each_block_size; 
            char buffer[each_block_size - 2];            
            memcpy(buffer, user_input,each_block_size - 1);
            printf("block 1\n");
            printf("[%s]\n",buffer);
            printf("\n");
            printf("block 2\n");
            int end_index_from_loop;                        
            for (int i = 1; i < blocks_need; i++){            
                int index_start = i * each_block_size;
                int index_end = index_start + each_block_size;            
                memcpy(buffer, &user_input[index_start],each_block_size);
                printf("[%s]\n",buffer);
                end_index_from_loop = index_end;
            }
            if(remainder == 0){
                 return 0;
            }else{
                printf("block last\n");
                memcpy(buffer, &user_input[end_index_from_loop],each_block_size);
                printf("[%s]\n",buffer);
            } 
            printf("\n");            
            /*                                           
            write_disk(meda_block + free_block_id_data,filename);
            write_disk(data_block_entry_index + i,user_input);
           
           */