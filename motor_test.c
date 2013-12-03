/*
 
*/ 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_wii.h"
#include "m_rf.h"

#include "localization.h"
#include "motor.h"

#define RIGHT 0
#define LEFT 1

#define CHANNEL 1
#define RXADDRESS 0x53 //52,53,54  recv for team 5
#define TXADDRESS 0xDA
#define PACKET_LENGTH 10

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

// Game commands
#define COMM_TEST 0xA0
#define PLAY 0xA1
#define GOAL_A 0xA2
#define GOAL_B 0xA3
#define PAUSE 0xA4
#define HALF 0xA5
#define FINISH 0xA7
#define ENEMEY 0xA8

volatile bool play = false;

// buffer for m_wii data
signed char loc[2] = {0,0};
signed char *x = &loc[0]; signed char *y = &loc[1];
int theta = 0; // variable to hold orientation in degrees

// Define functions
void timer1_setup(void);

void init()
{
	// Define output pins
	set(DDRB,MOTOR_EN);
	set(DDRB,LMOTOR_D);
	set(DDRB,RMOTOR_D);

}

int main(void)
{
	m_clockdivide(0); // 16MHz
	init();	
	m_usb_init(); // initialize usb
	timer1_setup(); // setup timer 1
	
	// Setup wireless module
	m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH);
	sei();//enable global interrupts
	int b = m_wii_open(); // initialize m_wii
	
	m_green(OFF);
	m_red(OFF);
	if(DEBUG == 1)
	{
		while(!m_usb_isconnected()); //wait
	}
	m_green(ON);	
	
    while(1)
    {
		 fwd();
		 m_wait(2000);
		 rev();
		 m_wait(2000);
		 right();
		 m_wait(2000);
		 left();
		 m_wait(2000);
		 stop();
    }
}

// Received packet
ISR(INT2_vect) {
	m_green(ON);
	m_red(OFF);
	m_rf_read(buffer, PACKET_LENGTH);
	m_usb_tx_string("\nrecv: ");		
	if (buffer[0] == PLAY)
		play = true;
	else if (buffer[0] == PAUSE)
		stop();
}

void timer1_setup(void){
	//Timer 1 config
	set(DDRB,6); //Set B6 to output
	set(TCCR1B,WGM13); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,WGM12); // ^
	set(TCCR1A,WGM11); // ^
	set(TCCR1A,WGM10); // ^
	set(TCCR1A,COM1B1); //Set at OCR1B, clear at rollover
	set(TCCR1A,COM1B0); // ^
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 20000; // Default frequency: 800Hz
	OCR1B = 18000; // ^ duty cycle: 10%
}

ISR(TIMER1_COMPA_vect){
	clear(MOTOR,MOTOR_EN);
}

ISR(TIMER1_COMPB_vect){
	set(MOTOR,MOTOR_EN);
}
