#ifndef DLLHASHTABLE_
#define DLLHASHTABLE_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj1lib.h"

struct HashMap{
  RTOS_TMR *RTOSTmrListPtr;            //head of DLL of timers
  int RTOSTmrListEntries;          //current number of entries in the list, starts at 0;
  INT32U RTOSTmrTickCtr;           //number of times the timer task has been signaled, starts at 0, basically a global count
};

extern struct HashMap *hashTable;

void insertToHash(RTOS_TMR *tmr);
void deleteFromHash(RTOS_TMR *tmr);

//dont really need search or display functions

#endif //DLLHASHTABLE_
