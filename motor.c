
#include "motor.h"

void fwd(void){
	set(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
	set(MOTOR,MOTOR_EN);
}

void rev(void){
	clear(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
	set(MOTOR,MOTOR_EN);
}

void right(void){
	set(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
	set(MOTOR,MOTOR_EN);
}

void left(void){
	clear(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
	set(MOTOR,MOTOR_EN);
}

void stop(void){
	clear(MOTOR,MOTOR_EN);
}
