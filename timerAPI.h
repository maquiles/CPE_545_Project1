#ifndef TIMERAPI_
#define TIMERAPI_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"proj1lib.h"
#include"DLLHashTable.h"

extern RTOS_TMR_PERR perr_handler;           //keep track of timer errors

RTOS_TMR_PERR check_error();

RTOS_TMR *RTOSTmrCreate(INT32U dly, INT32U period, RTOS_TMR_OPT opt, RTOS_TMR_CALLBACK callback, void *callback_arg, INT8U *pname, RTOS_TMR_PERR *perr);

BOOLEAN RTOSTmrDel(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr);

INT8U RTOSTmrNameGet(RTOS_TMR *ptmr, INT8U **pdest, RTOS_TMR_PERR *perr);

INT32U RTOSTmrRemainGet(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr);

INT8U RTOSTmrStateGet(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr);

BOOLEAN RTOSTmrStart(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr);

BOOLEAN RTOSTmrStop(RTOS_TMR *ptmr, RTOS_TMR_STOP_OPT opt, void *callback_arg, RTOS_TMR_PERR *perr);

RTOS_TMR_PERR RTOSTmrSignal(void);

#endif //TIMERAPI_
