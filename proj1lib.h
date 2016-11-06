#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#ifndef PROJ1LIB_
#define PROJ1LIB_

typedef unsigned char INT8U;
typedef unsigned int INT32U;
typedef enum {RTOS_FALSE, RTOS_TRUE} BOOLEAN;
typedef void (*RTOS_TMR_CALLBACK)(void *param);
typedef enum {RTOS_TMR_TYPE} TMR_TYPE;
typedef enum {RTOS_TMR_STATE_UNUSED, RTOS_TMR_STATE_RUNNING, RTOS_TMR_STATE_STOPPED, RTOS_TMR_STATE_COMPLETED} RTOS_TMR_STATE;
typedef enum {RTOS_TMR_OPT_ONE_SHOT, RTOS_TMR_OPT_PERIODIC} RTOS_TMR_OPT;
typedef enum {RTOS_TMR_OPT_NONE, RTOS_TMR_OPT_CALLBACK, RTOS_TMR_OPT_CALLBACK_ARG} RTOS_TMR_STOP_OPT;
typedef enum {RTOS_ERR_NONE,                //the call was successful
              RTOS_ERR_TMR_INVALID_DLY,     //you specified an invalid delay
              RTOS_ERR_TMR_INVALID_PERIOD,  //you specified an invalid period
              RTOS_ERR_TMR_INVALID_OPT,     //you specified an invalid option
              RTOS_ERR_TMR_NON_AVAIL,       //if there r no free timers from timer pool
              RTOS_ERR_TMR_INVALID,         //'ptmr'  is a NULL pointer
              RTOS_ERR_TMR_INVALID_TYPE,    //'ptmr'  is not pointing to an RTOS_TMR
              RTOS_ERR_TMR_INACTIVE,        //if the timer was not created
              RTOS_ERR_TMR_INVALID_STATE,   //the timer is in an invalid state
              RTOS_ERR_TMR_INVALID_DEST,    //'pdest' is a NULL pointer
              RTOS_ERR_TMR_STOPPED,         //if the timer was already stopped
              RTOS_ERR_TMR_NO_CALLBACK      //if the timer does not have a callback function defined}
              } RTOS_TMR_PERR;

typedef struct os_tmr{
    TMR_TYPE            RTOSTmrType;            /* Should always be set to RTOS_TMR_TYPE for timers */
    RTOS_TMR_CALLBACK   RTOSTmrCallback;        /* Function to call when timer expires */
    void                *RTOSTmrCallbackArg;    /* Argument to callback function */
    void                *RTOSTmrNext;           /* Double link list pointers   */
    void                *RTOSTmrPrev;
    INT32U              RTOSTmrMatch;           /* Timer expires when RTOSTmrTickCtr = RTOSTmrMatch*/
    INT32U              RTOSTmrDly;             /* Delay time before periodic update starts */
    INT32U              RTOSTmrPeriod;          /* Period to repeat timer  */
    INT8U               *RTOSTmrName;           /* Name to give the timer   */
    RTOS_TMR_OPT        RTOSTmrOpt;             /* Options (see RTOS_TMR_OPT_xxx)  */
    RTOS_TMR_STATE      RTOSTmrState;           /* Indicates the state of the timer:*/
                                                /*     RTOS_TMR_STATE_UNUSED   */
                                                /*     RTOS_TMR_STATE_RUNNING  */
                                                /*     RTOS_TMR_STATE_STOPPED  */
    //RTOS_TMR_PERR       RTOSTmrPerr;            /* Make sure there are no errors */
} RTOS_TMR;

#endif //PROJ1LIB_
