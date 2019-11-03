#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



typedef struct node{

	int data;
    char word[15];
	struct node* previous;

}*node_ptr;


node_ptr front = NULL;
node_ptr rear = NULL;

bool is_empty(){

	return (front == NULL);
}

void display_front(){

	if(is_empty()){

		printf("\nThe queue is empty!\n");
		return;
	}

	printf("\n[%d]\n", front->data);
}

void display_rear(){

	if(is_empty()){

		printf("\nThe queue is empty!\n");
		return;
	}
	
	printf("\n[%d]\n", rear->data);
}

bool enqueue(int value, char *string){

	node_ptr item = (node_ptr) malloc(sizeof(struct node));

	if (item == NULL)
		return false;

	item->data = value;
     
    strcpy(item->word,string);
	item->previous = NULL;

	if(rear == NULL)
		front = rear = item;
	else{
		rear->previous = item;
		rear = item;
	}

	return true;
}

bool dequeue(){

	if(is_empty()){

		//printf("\nThe queue is empty!\n");
		return false;
	}

	node_ptr temp = front;
	front = front->previous;
	free(temp);

	return true;
}

void display(){


	if(is_empty()){

		printf("\nThe queue is empty!\n");
		return;
	}

	node_ptr temp = front;

	printf("\n[front -> ");

	while(temp != NULL){
		printf("[%d]", temp->data);
        printf("[%s]", temp->word);
		temp = temp->previous;
	}

	printf(" <- rear]\n");

}

int queue_get_size(){
    
    int number = 0;
    if(is_empty()){
        printf("The size is [%d]\n",number);
        return number;
    }

    node_ptr temp = front;

    while(temp != NULL){
        number++;
        temp = temp->previous;
    }

    printf("The size is [%d]\n",number);
    return number;
}

bool clear(){

	if(is_empty()){

		printf("\nThe queue is empty!\n");
		return false;
	}

	node_ptr current = front;
	node_ptr previous = NULL;

	while(current != NULL){

		previous = current->previous;
		free(current);
		current = previous;
	}

	front = NULL;

	return is_empty();
}

