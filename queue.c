#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct{
	int volume_x;
	int volume_y;
} CupState;

struct Node{

	struct Node * next;
};

struct typedef{
	struct Node * head;
	struct Node * tail;
	int size;
} Queue;

struct Queue queue_initialize(){
	CupState zero_cup_state = {0,0};
	struct Queue new_queue = {NULL , NULL , zero_cup_state};
	return zero_cup_state;
}

int queue_size(struct Queue * current_queue){
	return current_queue->size;
}

int queue_front(struct Queue * current_queue){
	if(!current_queue->head)
		return 0;
	else 
		return (current_queue->head)->value;
}

void queue_insert(struct Queue * current_queue , int new_value){
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

void queue_pop(struct Queue * current_queue){
	struct Node * current_head_node = current_queue->head;
	current_queue->head = current_head_node->next;
	free(current_head_node);

	if(!current_queue->head)
		current_queue->tail = NULL;

	current_queue->size--;
}

int main()
{

}