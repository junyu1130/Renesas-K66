#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "headfile.h"

#define TRIG_Port		E8
#define TRIG				PEout(8)
#define ECHO_Port		E9
#define ECHO				PEin(9)

extern uint32_t  Distance;
extern uint8_t speedUpFlag;

void HC_SR04_Init(void);  
void distance(void);
void distanceControl(void);

#endif
