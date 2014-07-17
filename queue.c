#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct{
	int volume_x;
	int volume_y;
} CupState;

struct Node{
	CupState value;
	struct Node * next;
};

typedef struct{
	struct Node * head;
	struct Node * tail;
	int size;
} Queue;

Queue queue_initialize(){
	CupState zero_cup_state = {0,0};
	Queue new_queue = {NULL , NULL , 0};
	return new_queue;
}

int queue_size(Queue * current_queue){
	return current_queue->size;
}

CupState queue_front(Queue * current_queue){
	if(!current_queue->head){
		CupState zero_cup_state = {0,0};
		return zero_cup_state;
	}
	else 
		return (current_queue->head)->value;
}

void queue_insert(Queue * current_queue , const CupState new_value){
	struct Node * ptr_new_node = malloc(sizeof (struct Node));

	ptr_new_node->value = new_value;
	ptr_new_node->next = NULL;

	if(current_queue->tail){
		struct Node * current_last_node = current_queue->tail;
		current_last_node->next = ptr_new_node;
	}

	current_queue->tail = ptr_new_node;

	if(!current_queue->head)
		current_queue->head = ptr_new_node;

	current_queue->size++;
}

void queue_pop(Queue * current_queue){
	struct Node * current_head_node = current_queue->head;
	current_queue->head = current_head_node->next;
	free(current_head_node);

	if(!current_queue->head)
		current_queue->tail = NULL;

	current_queue->size--;
}

int main(int args , char * argv[])
{
	Queue q = queue_initialize();
	int i , n;
	n = 5;
	for(i = 0 ; i < n ; i++){
		CupState x = {i,i};
		queue_insert(&q , x);
	}
	for(i = 0 ; i < n ; i++){
		CupState x = queue_front(&q);
		printf("%d %d\n" , x.volume_x , x.volume_y);
		queue_pop(&q);
	}
	return 0;
}