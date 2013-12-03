#include "motor.h"

void fwd(void){
	set(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
}

void rev(void){
	clear(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
}

void right(void){
	set(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
}

void left(void){
	clear(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
}

void stop(void){
	clear(MOTOR,MOTOR_EN);
	OCR1B = OCR1A;
}
