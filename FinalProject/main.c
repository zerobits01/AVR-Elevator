/*
 * FinalProject.c
 *
 * Created: 7/15/2020 8:10:32 PM
 * Author : zerobits01
 * Description : 
 *	4 sensors for detecting floor ??
 *	4 buttons inside -/
 *	4 buttons outside -/
 *	1 7segment for showing the floor -/
 *	4 output for :
 *		door locks when the cage is on other floors -/
 *		the other floors doors should be closed -/
 *	the time for staying in each floor is arbitrary
 *
 *	so what are the sensors for detecting the floor ?
 # next steps : 
 # closing and opening the doors more complete
 # after arriving opening the door
 # after some seconds door should be close
 # sensors for the floor detection
 # i have to use timer for door close and a counter.
*/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
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
void deQueue(struct Queue* q)
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
	
	free(temp);
}



// define global variables here
// common cathode
const char sevseg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};// now i use common cathode because i wanna isolate pin7

char floor_number = 1; // 0 1 2 3
char next_floor = 1;
struct Queue* q;

char flg;


// define ISRs here

// ISR for blinking seven segment
ISR (TIMER0_OVF_vect)
{
	if(flg){
		PORTC = sevseg[next_floor];
		flg = 0;
	}else{
		PORTC = 0;
		flg = 1;
	}
}

// ISR for floor detection
ISR (INT1_vect)
{
	switch(PINA){
		case 0b11111110 :{
			floor_number = 1;
		}	break;
		case 0b11111101 :{
			floor_number = 2;
		}	break;
		case 0b11111011 :{
			floor_number = 3;
		}	break;
		case 0b11110111 :{
			floor_number = 4;
		}	break;
	}
	PORTA = 0xFF;
	if(next_floor == floor_number){
		PORTD &= (255 << 2); // stop the motor
		PORTD &= (~(1 << (next_floor + 3))); // open the door lock for the current floor
	}
	TCCR0 = 0;
	PORTC = sevseg[floor_number];

}


// ISR for buttons detection
ISR (INT0_vect)
{
	switch(PINB){
		case 0b11111110 :{
			next_floor = 1;
		}	break;
		case 0b11111101 :{
			next_floor = 2;
		}	break;
		case 0b11111011 :{
			next_floor = 3;
		}	break;
		case 0b11110111 :{
			next_floor = 4;
		}	break;
		case 0b11101111 :{
			next_floor = 1;
		}	break;
		case 0b11011111 :{
			next_floor = 2;
		}	break;
		case 0b10111111 :{
			next_floor = 3;
		}	break;
		case 0b01111111 :{
			next_floor = 4;
		}	break;
				
	}
	

	if(floor_number < next_floor){
		PORTD = 0x01;
		PORTD |= 0xF0; // closing all the doors
		}else if( floor_number == next_floor){
		PORTD |= 0xF0; // closing all the doors
		PORTD &= (~(1 << (next_floor + 3))); // open the door lock
		}else{
		PORTD = 2;
		PORTD |= 0xF0; // closing all door locks
	}
	
	TCCR0 = 0x05;

}


void setup(){
	// TODO : all the setup for ports and configs here
	DDRA = 0xF0; // define as input for sensors
	PORTA = 0x0F;
	DDRB = 0x00; // define as input for input keys
	PORTB = 0xFF; // set the pull up enable for keys
	DDRC = 0xFF; // set as output for seven segment
	DDRD = 0xF3; // last two bits output for motor control other bits input
	PORTB = 0xFF; // setting pull up and test for the motor
	
	PORTD = 0b11110011;
	
	GICR |= (1 << INT0); // enabling interrupt 0
	MCUCR |= ((1 << ISC01) | (1 << ISC00)); // setting the interrupt 0 to detect rising edge
	
	GICR |= (1 << INT1); // enabling interrupt 0
	MCUCR |= ((1 << ISC11) | (1 << ISC10)); // setting the interrupt 0 to detect rising edge
	
	TIMSK = 0x01;
	
	// really i can say now i am a good avr developer
	sei(); // enabling global interrupt	
	
	PORTC = sevseg[next_floor];
	
	q = createQueue();
}



int main(void)
{
	
	setup();
    /* Replace with your application code */
    while (1) 
    {
    }
}

