#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include"proj1lib.h"
#include"DLLHashTable.h"
#include"timerAPI.h"

RTOS_TMR_PERR *perr = &perr_handler;
int runtime = 0;

void func1(){
  printf("This is function2 1 an the time is %d\n", runtime);
}
void func2(){
  printf("This is function 2 an the time is %d\n", runtime);
}
void func3(){
  printf("This is function 3 an the time is %d\n", runtime);
}

int main(){
  //do the test with the 3 timers from the end of the Project 1 Document on Canvas
  INT8U *tmr1 = "TIMER1";
  INT8U *tmr2 = "TIMER2";
  INT8U *tmr3 = "TIMER3";

  RTOS_TMR *timer1 = RTOSTmrCreate(5, 5, RTOS_TMR_OPT_PERIODIC, func1, NULL, tmr1, perr);
  RTOS_TMR *timer2 = RTOSTmrCreate(3, 3, RTOS_TMR_OPT_PERIODIC, func2, NULL, tmr2, perr);
  RTOS_TMR *timer3 = RTOSTmrCreate(10, 0, RTOS_TMR_OPT_ONE_SHOT, func3, NULL, tmr3, perr);

  
}
