#ifndef localization_h
#define localization_h


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_wii.h"

#define DEBUG 0

// Define functions
int localize(signed char location[]); // function to find position and orientation

#endif
