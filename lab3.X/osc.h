/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OSC_H
#define OSC_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

void initOsc(uint8_t IRCF);

#endif	/* OSC_H */

