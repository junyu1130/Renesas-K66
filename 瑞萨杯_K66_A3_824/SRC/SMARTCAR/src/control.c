#include "control.h"

void PID_init(PID *p, float setpoint, float kp, float ki, float kd)                                       //PID?????
{
    p->wantout = setpoint;
    p->ActualValue = 0;
    p->Kp = kp;
    p->Ki = ki;
    p->Kd = kd;
    p->err = 0;
    p->err_next = 0;
    p->err_last = 0;
}

int PID_Calc(PID *p, int protect)         //protect=0£¬ÏÞÖÆ¹Øµô                                                                                   //PID??
{
    float du;
    p->err = p->wantout - p->ActualValue;
    du = p->Kp * (p->err - p->err_next) + p->Ki * p->err + p->Kd * (p->err - 2 * p->err_next + p->err_last);
    p->err_last = p->err_next;
    p->err_next = p->err;
    if(protect != 0)
    {
        if(du > protect)
            du = protect;
        if(du < (-protect))
            du = -protect;
    }
    return   (int)du;
}
