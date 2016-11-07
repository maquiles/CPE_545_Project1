#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include"proj1lib.h"
#include"DLLHashTable.h"
#include"timerAPI.h"

RTOS_TMR_PERR *perr = &perr_handler;
int RTOSTmrTickCtr = 0;

void func1(){
  printf("This is function2 1 an the time is %d\n", hashTable->RTOSTmrTickCtr);
}
void func2(){
  printf("This is function 2 an the time is %d\n", hashTable->RTOSTmrTickCtr);
}
void func3(){
  printf("This is function 3 an the time is %d\n", hashTable->RTOSTmrTickCtr);
}

void RTOSTmrTask(RTOS_TMR *tmr){
  while(RTOSTmrStateGet(tmr, perr) == RTOS_TMR_STATE_RUNNING){
    if(RTOSTmrRemainGet(tmr, perr) == 0){
        if(tmr->RTOSTmrOpt == RTOS_TMR_OPT_ONE_SHOT){
          RTOSTmrStop(tmr, RTOS_TMR_OPT_CALLBACK, NULL, perr);
          //stop function should execute the callback function
        }
        else{//should be periodic or there will be an error
          tmr->RTOSTmrCallback(NULL);
          tmr->RTOSTmrMatch = hashTable->RTOSTmrTickCtr + tmr->RTOSTmrPeriod;
        }
    }
    else{
      //pthread_cond_wait();
    }

    hashTable->RTOSTmrTickCtr++;
  }
}

int main(){
  //do the test with the 3 timers from the end of the Project 1 Document on Canvas
  INT8U *tmr1 = "TIMER1";
  INT8U *tmr2 = "TIMER2";
  INT8U *tmr3 = "TIMER3";

  RTOS_TMR *timer1 = RTOSTmrCreate(5, 5, RTOS_TMR_OPT_PERIODIC, func1, NULL, tmr1, perr);
  RTOS_TMR *timer2 = RTOSTmrCreate(3, 3, RTOS_TMR_OPT_PERIODIC, func2, NULL, tmr2, perr);
  RTOS_TMR *timer3 = RTOSTmrCreate(10, 0, RTOS_TMR_OPT_ONE_SHOT, func3, NULL, tmr3, perr);

  RTOSTmrStart(timer1, perr);
  RTOSTmrStart(timer2, perr);
  RTOSTmrStart(timer3, perr);

  pthread_t task1;
  pthread_t task2;
  pthread_t task3;

  pthread_create(task1, NULL, RTOSTmrTask, timer1);
  pthread_create(task2, NULL, RTOSTmrTask, timer2);
  pthread_create(task3, NULL, RTOSTmrTask, timer3);
}
