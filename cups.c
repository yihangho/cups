#include <stdio.h>
#include <stdlib.h>
typedef long long int;

// Code for the linked list and queue
typedef struct{
	int value;
	Node * next;
} Node;

typedef struct{
	Node * head;
	Node * tail;
} Queue;

Queue initializeQueue(){
	Queue new_queue = {NULL , NULL};
	return new_queue
}

int queue_front(Queue * current_queue){
	return (current_queue->head)->value;
}

void queue_pop(Queue * current_queue){
	Node * new_first_node = (current_queue->head)->next;
	free(current_queue->head);
	current_queue->head = new_first_node;
	if(current_queue->head == NULL)
		current_queue->tail = NULL;
}

void queue_insert(Queue * current_queue , int new_value){
	Node * new_node = malloc(sizeof(*new_node));
	new_node->value = new_value;
	new_node->next = NULL;
	(current_queue->tail)->next = new_node;
	current_queue->tail = new_node;
	if(!current_queue->head)
		current_queue->head = current_queue->tail;
}

bool queue_empty(Queue * current_queue){
	return (current_queue->head == NULL) || (current_queue->tail == NULL);
}

// End of linked list code and queue

int MAX_VOLUME_X;
int MAX_VOLUME_Y;
int TARGET_VOLUME;

int minimum(const int a, const int b){
	if(a < b) return a;
	else return b;
}

typedef struct{
	int volume_x;
	int volume_y;
} CupState;

CupState * pour_from_y_to_x(const CupState * current_state){ //take note that this returns a pointer!
	CupState * new_state = malloc(sizeof(*new_state));
	new_state->volume_x = current_state->volume_x;
	new_state->volume_y = current_state->volume_y;

	new_state->volume_x = minimum(new_state->volume_x + new_state->volume_y , MAX_VOLUME_X);
	new_state->volume_y -= (new_state->volume_x - current_state->volume_x); 
	return new_state;
}

CupState * pour_from_x_to_y(const CupState * current_state){ //take note that this returns a pointer!
	CupState * new_state = malloc(sizeof(*new_state));
	new_state->volume_x = current_state->volume_x;
	new_state->volume_y = current_state->volume_y;

	new_state->volume_y = minimum(new_state->volume_y + new_state->volume_y , MAX_VOLUME_Y);
	new_state->volume_y -= (new_state->volume_x - current_state->volume_x); 
	return new_state;	
}

CupState * empty_x(const CupState * current_state){
	CupState * new_state = malloc(sizeof(*new_state));
	new_state->volume_x = 0;
	new_state->volume_y = current_state->volume_y;
	return new_state;
}

CupState * empty_y(const CupState * current_state){
	CupState * new_state =malloc(sizeof(*new_state));
	new_state->volume_y = 0;
	new_state->volume_x = current_state->volume_x;
	return new_state;
}

CupState * fill_x(const CupState * current_state){
	CupState * new_state = malloc(sizeof(*new_state));
	new_state->volume_x = MAX_VOLUME_X;
	new_state->volume_y = current_state->volume_y;
	return new_state;
}

CupState * fill_y(const CupState * current_state){
	CupState * new_state = malloc(sizeof(*new_state));
	new_state->volume_y = MAX_VOLUME_Y;
	new_state->volume_x = current_state->volume_x;
	return new_state;
}

int main(int args , char * argv[])
{
	if(args == 3){
		MAX_VOLUME_X = atoi(argv[1]);
		MAX_VOLUME_Y = atoi(argv[2]);
		TARGET_VOLUME = atoi(argv[3]);
	}
	else {
		MAX_VOLUME_X = 9;
		MAX_VOLUME_Y = 4;
		TARGET_VOLUME = 2;
	}



	return 0;
}