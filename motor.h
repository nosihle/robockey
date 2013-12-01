#ifndef motor_h
#define motor_h

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_wii.h"

// M2 motor outputs
#define MOTOR PORTB
#define MOTOR_EN 0
#define LMOTOR_D 1 
#define RMOTOR_D 2

void fwd(void);
void rev(void);
void left(void);
void right(void);
void stop(void);

#endif
