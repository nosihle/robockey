#include <avr/io.h>
#include <avr/interrupt.h>
#include "m_general.h"
#include "m_usb.h"


int ir[6] = {0,0,0,0,0,0}; // Array to hold IR adc values
int *FL = &ir[0]; int *FC = &ir[1]; int *FR = &ir[2];
int *BL = &ir[3]; int *BC = &ir[4]; int *BR = &ir[5];
int F_REG = 0;

void adc_setup(void);

volatile int FLAG = 0; // is set when all adc values are read

int main(void)
{
	m_red(ON);
	m_usb_init();
	while(!m_usb_isconnected()); // wait for a serial msg
	m_red(OFF);
	
	m_clockdivide(0); // set clock frequency to 16MHz
	
	adc_setup();
	m_disableJTAG();
	set(ADCSRA,ADEN);//enable ADC
	
	sei();//enable global interrupts
	
	set(ADCSRA,ADSC);//start conversion
	while(1){
		if(FLAG){ // compare readings and move motor accordingly
			m_green(TOGGLE);
			clear(ADCSRA,ADEN); // Disable ADC
			
			m_usb_tx_string("\nF0:");
			m_usb_tx_int(ir[0]);
			m_usb_tx_string("\tF1: ");
			m_usb_tx_int(ir[1]);
			m_usb_tx_string("\tF4: ");
			m_usb_tx_int(ir[2]);
			
			FLAG = 0;
			set(ADCSRA,ADEN); // enable ADC again
			set(ADCSRA,ADSC);//start next conversion
		}
	}
}

//Interrupt called when conversion completes
ISR(ADC_vect){
	clear(ADCSRA,ADEN); // Disable ADC
	ir[F_REG] = ADC;
	if(F_REG == 2){
		F_REG = 0;
		FLAG = 1;
	}else
		F_REG++;
	//m_usb_tx_int((value*(5.0/1023.0)*100.0));
	if(F_REG == 0){ // Set ADC to F0
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}else if(F_REG == 1){ // Set ADC to F1
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
	}else if(F_REG == 2){ // Set ADC to F4
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}

	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC);//start next conversion
}

void adc_setup(void){
	 // set prescaler frequency to 16MHz / 128
	set(ADCSRA,ADPS2); // ^^
	set(ADCSRA,ADPS1); // ^^
	set(ADCSRA,ADPS0); // ^^
	clear(ADMUX,REFS1); // set the reference voltage to Vcc
	set(ADMUX,REFS0); // ^
	set(ADCSRA,ADIE); // enable interrupts for ADC
	set(DIDR0,ADC0D); // disable digital input on F0
	set(DIDR0,ADC1D); // disable digital input on F1
	set(DIDR0,ADC4D); // disable digital input on F4
}
