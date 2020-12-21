#ifndef __FINDTRACK_H__
#define __FINDTRACK_H__

#include "headfile.h"

#define MidLine 92
#define effective_left 35
#define effective_right (COL-38)
#define StartRow (ROW-3)

/*** �ⲿ������������ ****/  
extern int   RightEdge[ROW];
extern int   LeftEdge[ROW];
extern int   MiddleLine[ROW];
extern int   Width[ROW];
extern int   LeftLose;
extern int   RightLose;
extern int   AllLose ;

/*** �������� ****/    
void findTrack(void);


#endif
