#ifndef _CONTROL_H
#define _CONTROL_H

#include "headfile.h"

typedef struct PID                          
{
  float wantout;                               
	float ActualValue;                      
	float Ki;                 			             
  float Kp;                                         
  float Kd;                                        
  int err;                                       
	int err_next;							    
	int err_last;				
} PID;

extern PID *pid_speed_L, *pid_speed_R;
extern PID *p_pid_servo;
extern PID *p_pid_distance;

int PID_Calc(PID *p , int protect);
void PID_init(PID *p, float setpoint, float kp ,float ki, float kd);

#endif
