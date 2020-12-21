#ifndef _READ_DATA_H_
#define _READ_DATA_H_

#include "headfile.h"

#define parallelData0_Port		    B9
#define parallelData0				      PBin(9)
#define parallelData1_Port		    C16
#define parallelData1				      PCin(16)
#define parallelData2_Port		    C17
#define parallelData2				      PCin(17)
#define parallelData3_Port		    A29
#define parallelData3				      PAin(29)
#define parallelData4_Port		    E12
#define parallelData4				      PEin(12)
#define parallelData5_Port		    D0
#define parallelData5				      PDin(0)
#define parallelData6_Port		    D1
#define parallelData6				      PDin(1)
#define parallelData7_Port		    D2
#define parallelData7				      PDin(2)
#define parallelData8_Port		    D3
#define parallelData8				      PDin(3)
#define parallelData9_Port		    D4
#define parallelData9				      PDin(4)
#define parallelData10_Port		    D5
#define parallelData10			      PDin(5)
#define parallelData11_Port		    D6
#define parallelData11			      PDin(6)
#define parallelData12_Port		    D7
#define parallelData12			      PDin(7)

void readData_init(void);//并口端口初始化
void dataReadAndAnalyze(void);//数据读取与解析

#endif
