#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"



int get_free_space(int list[]){

    for (int i = 0; i < file_list_number; i++)
    {
        if(list[i] == 0){
            return i;
        }
    }
    printf("The spcae is full now\n");
    // all full return -1;
    return -1;
}


int main(){

    int list[10] = {1,2,3,4,5,2,2,3,4,9};
    int value = get_free_space(list);
    printf("%d\n",value);

    

    return 0;
}