#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct Node{
	int value;
	struct Node * next;
};

struct Queue{
	struct Node * head;
	struct Node * tail;
};

struct Queue queue_initialize(){
	struct Queue new_queue = {NULL , NULL};
	return new_queue;
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
}

void queue_pop(struct Queue * current_queue){
	struct Node * current_head_node = current_queue->head;
	current_queue->head = current_head_node->next;
	free(current_head_node);

	if(!current_queue->head)
		current_queue->tail = NULL;
}

int queue_front(struct Queue * current_queue){
	Node * current_head_node = current_queue->head;
	return current_head_node->value;
}

// void queue_pop(Queue * current_queue){
// 	Node * new_first_node = (current_queue->head)->next;
// 	free(current_queue->head);
// 	current_queue->head = new_first_node;
// 	if(current_queue->head == NULL)
// 		current_queue->tail = NULL;
// }


// bool queue_empty(Queue * current_queue){
// 	return (current_queue->head == NULL) || (current_queue->tail == NULL);
// }

int main()
{
	struct Queue q = queue_initialize();
	queue_insert(&q , 1);
}