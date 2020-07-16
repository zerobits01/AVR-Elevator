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

// define global variables here
// common cathode
const char sevseg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};// now i use common cathode because i wanna isolate pin7
char floor_number = 1; // 0 1 2 3
char next_floor = 1;
// define ISRs here
// i have to define int0 to rising edge
// also in the ISR checking that which button has been choice from the port B

// ISR
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
}

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
	PORTC = sevseg[next_floor]; // floor number should be set at other ISR
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
	
	// really i can say now i am a good avr developer
	sei(); // enabling global interrupt	
}



int main(void)
{
	
	setup();
    /* Replace with your application code */
    while (1) 
    {
    }
}

