#include <stdlib.h>

// A linked list (LL) node to store a queue entry
struct QNode {
	int key;
	struct QNode* next;
};


// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
	struct QNode *front, *rear;
};


// define global variables here
// common cathode
const char sevseg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};// now i use common cathode because i wanna isolate pin7

char floor_number = 1; // 0 1 2 3
char next_floor = 1;
struct Queue* q;

char flg, flg0;


// A utility function to create a new linked list node.
struct QNode* newNode(int k)
{
	struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->key = k;
	temp->next = NULL;
	return temp;
}


// A utility function to create an empty queue
struct Queue* createQueue()
{
	struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
}


// The function to add a key k to q
void enQueue(struct Queue* q, int k)
{
	// Create a new LL node
	struct QNode* temp = newNode(k);
	
	// If queue is empty, then new node is front and rear both
	if (q->rear == NULL) {
		q->front = q->rear = temp;
		return;
	}
	
	// Add the new node at the end of queue and change rear
	q->rear->next = temp;
	q->rear = temp;
}


// Function to remove a key from given queue q
struct QNode* deQueue(struct Queue* q)
{
	// If queue is empty, return NULL.
	if (q->front == NULL)
		return;
	
	// Store previous front and move front one node ahead
	struct QNode* temp = q->front;
	
	q->front = q->front->next;
	
	// If front becomes NULL, then change rear also as NULL
	if (q->front == NULL)
	q->rear = NULL;
	
	return temp;
}


char containsInQueue(char ind){
	
	// If queue is empty, return NULL.
	if (q->front == NULL)
		return 0;
		
	// Store previous front and move front one node ahead
	struct QNode* temp = q->front;
	while(1){
		if(temp->key == ind){
			return 1;
		}
		if(temp->next != NULL){
			break;
		}
		temp = temp->next;
	}
	return 0;
}


char queueIsEmpty(struct Queue* q){
	if(q->front->next == NULL)
		return 1;
	
	return 0;
}

