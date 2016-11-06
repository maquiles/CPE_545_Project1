#include<stdio.h>
#include<stdlib.h>
#include"proj1lib.h"
#include"DLLHashTable.h"
#include"timerAPI.h"

INT8U perr_handler;           //keep track of timer errors

/*
 ************************************************************************************************************************
 *                                                   CREATE A TIMER
 *
 * Description: This function is called by your application code to create a timer.
 *
 * Arguments  : dly           Initial delay.
 *                            If the timer is configured for ONE-SHOT mode, this is the timeout used
 *                            If the timer is configured for PERIODIC mode, this is the first timeout to wait for
 *                               before the timer starts entering periodic mode
 *
 *              period        The 'period' being repeated for the timer.
 *                               If you specified 'RTOS_TMR_OPT_PERIODIC' as an option, when the timer expires, it will
 *                               automatically restart with the same period.
 *
 *              opt           Specifies either:
 *                               RTOS_TMR_OPT_ONE_SHOT       The timer counts down only once
 *                               RTOS_TMR_OPT_PERIODIC       The timer counts down and then reloads itself
 *
 *              callback      Is a pointer to a callback function that will be called when the timer expires.  The
 *                               callback function must be declared as follows:
 *
 *                               void MyCallback (RTOS_TMR *ptmr, void *p_arg);
 *
 *              callback_arg  Is an argument (a pointer) that is passed to the callback function when it is called.
 *
 *              pname         Is a pointer to an ASCII string that is used to name the timer.  Names are useful for
 *                               debugging.
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID_DLY     you specified an invalid delay
 *                               RTOS_ERR_TMR_INVALID_PERIOD  you specified an invalid period
 *                               RTOS_ERR_TMR_INVALID_OPT     you specified an invalid option
 *                               RTOS_ERR_TMR_NON_AVAIL       if there are no free timers from the timer pool
 *
 * Returns    : A pointer to an RTOS_TMR data structure.
 *              This is the 'handle' that your application will use to reference the timer created.
 ************************************************************************************************************************
 */

RTOS_TMR *RTOSTmrCreate(INT32U dly, INT32U period, RTOS_TMR_OPT opt, RTOS_TMR_CALLBACK callback, void *callback_arg, INT8U *pname, RTOS_TMR_PERR *perr){
    RTOS_TMR *timer = (RTOS_TMR*)malloc(sizeof(RTOS_TMR));

    timer->RTOSTmrType = RTOS_TMR_TYPE;
    timer->RTOSTmrDly = dly;
    timer->RTOSTmrPeriod = period;
    timer->RTOSTmrOpt = opt;
    timer->RTOSTmrCallback = callback;
    timer->RTOSTmrCallback = callback_arg;
    timer->RTOSTmrName = pname;
    timer->RTOSTmrNext = NULL;
    timer->RTOSTmrPrev = NULL;
    timer->RTOSTmrState = RTOS_TMR_STATE_UNUSED;
    timer->RTOSTmrMatch = dly; //this is only true when the timer is created

    insertToHash(timer); //add the newly created timer to the hash table

    return timer;
}

/*
 ************************************************************************************************************************
 *                                                   DELETE A TIMER
 *
 * Description: This function is called by your application code to delete a timer.
 *
 * Arguments  : ptmr          Is a pointer to the timer to stop and delete.
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID        'ptmr'  is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID_TYPE   'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE       if the timer was not created
 *                               RTOS_ERR_TMR_INVALID_STATE  the timer is in an invalid state
 *
 * Returns    : RTOS_TRUE       If the call was successful
 *              RTOS_FALSE      If not
 ************************************************************************************************************************
 */

BOOLEAN RTOSTmrDel(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr){
    free(ptmr);
    return RTOS_TRUE;
}

/*
 ************************************************************************************************************************
 *                                             GET THE NAME OF A TIMER
 *
 * Description: This function is called to obtain the name of a timer.
 *
 * Arguments  : ptmr          Is a pointer to the timer to obtain the name for
 *
 *              pdest         Is a pointer to pointer to where the name of the timer will be placed.
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE               The call was successful
 *                               RTOS_ERR_TMR_INVALID_DEST   'pdest' is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID        'ptmr'  is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID_TYPE   'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE       'ptmr'  points to a timer that is not active
 *                               RTOS_ERR_TMR_INVALID_STATE  the timer is in an invalid state
 *
 * Returns    : The length of the string or 0 if the timer does not exist.
 ************************************************************************************************************************
 */

INT8U RTOSTmrNameGet(RTOS_TMR *ptmr, INT8U **pdest, RTOS_TMR_PERR *perr){
    pdest = &(ptmr->RTOSTmrName);
    return sizeof(ptmr->RTOSTmrName)-1; //size of char -1 for NULL terminator
}

/*
 ************************************************************************************************************************
 *                                    GET HOW MUCH TIME IS LEFT BEFORE A TIMER EXPIRES
 *
 * Description: This function is called to get the number of ticks before a timer times out.
 *
 * Arguments  : ptmr          Is a pointer to the timer to obtain the remaining time from.
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID        'ptmr' is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID_TYPE   'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE       'ptmr' points to a timer that is not active
 *                               RTOS_ERR_TMR_INVALID_STATE  the timer is in an invalid state
 *
 * Returns    : The time remaining for the timer to expire.  The time represents 'timer' increments.  In other words, if
 *              RTOSTmr_Task() is signaled every 1/10 of a second then the returned value represents the number of 1/10 of
 *              a second remaining before the timer expires.
 ************************************************************************************************************************
 */

INT32U RTOSTmrRemainGet(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr){
    return (ptmr->RTOSTmrMatch) - hashTable->RTOSTmrTickCtr; //ticks to match - total ticks
}

/*
 ************************************************************************************************************************
 *                                    FIND OUT WHAT STATE A TIMER IS IN
 *
 * Description: This function is called to determine what state the timer is in:
 *
 *                  RTOS_TMR_STATE_UNUSED     the timer has not been created
 *                  RTOS_TMR_STATE_STOPPED    the timer has been created but has not been started or has been stopped
 *                  RTOS_TMR_COMPLETED        the timer is in ONE-SHOT mode and has completed it's timeout
 *                  RTOS_TMR_RUNNING          the timer is currently running
 *
 * Arguments  : ptmr          Is a pointer to the desired timer
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID        'ptmr' is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID_TYPE   'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE       'ptmr' points to a timer that is not active
 *                               RTOS_ERR_TMR_INVALID_STATE  if the timer is not in a valid state
 *
 * Returns    : The current state of the timer (see description).
 ************************************************************************************************************************
 */

INT8U RTOSTmrStateGet(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr){
    return ptmr->RTOSTmrState;
}

/*
 ************************************************************************************************************************
 *                                                   START A TIMER
 *
 * Description: This function is called by your application code to start a timer.
 *
 * Arguments  : ptmr          Is a pointer to an RTOS_TMR
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID
 *                               RTOS_ERR_TMR_INVALID_TYPE    'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE        if the timer was not created
 *                               RTOS_ERR_TMR_INVALID_STATE   the timer is in an invalid state
 *
 * Returns    : RTOS_TRUE    if the timer was started
 *              RTOS_FALSE   if an error was detected
 ************************************************************************************************************************
 */

BOOLEAN RTOSTmrStart(RTOS_TMR *ptmr, RTOS_TMR_PERR *perr){
    ptmr->RTOSTmrState = RTOS_TMR_STATE_RUNNING;
    return RTOS_TRUE;
}

/*
 ************************************************************************************************************************
 *                                                   STOP A TIMER
 *
 * Description: This function is called by your application code to stop a timer.
 *
 * Arguments  : ptmr          Is a pointer to the timer to stop.
 *
 *              opt           Allows you to specify an option to this functions which can be:
 *
 *                               RTOS_TMR_OPT_NONE          Do nothing special but stop the timer
 *                               RTOS_TMR_OPT_CALLBACK      Execute the callback function, pass it the callback argument
 *                                                        specified when the timer was created.
 *                               RTOS_TMR_OPT_CALLBACK_ARG  Execute the callback function, pass it the callback argument
 *                                                        specified in THIS function call
 *
 *              callback_arg  Is a pointer to a 'new' callback argument that can be passed to the callback function
 *                               instead of the timer's callback argument.  In other words, use 'callback_arg' passed in
 *                               THIS function INSTEAD of ptmr->RTOSTmrCallbackArg
 *
 *              perr          Is a pointer to an error code.  '*perr' will contain one of the following:
 *                               RTOS_ERR_NONE
 *                               RTOS_ERR_TMR_INVALID         'ptmr' is a NULL pointer
 *                               RTOS_ERR_TMR_INVALID_TYPE    'ptmr'  is not pointing to an RTOS_TMR
 *                               RTOS_ERR_TMR_INACTIVE        if the timer was not created
 *                               RTOS_ERR_TMR_INVALID_OPT     if you specified an invalid option for 'opt'
 *                               RTOS_ERR_TMR_STOPPED         if the timer was already stopped
 *                               RTOS_ERR_TMR_INVALID_STATE   the timer is in an invalid state
 *                               RTOS_ERR_TMR_NO_CALLBACK     if the timer does not have a callback function defined
 *
 * Returns    : RTOS_TRUE       If we stopped the timer (if the timer is already stopped, we also return RTOS_TRUE)
 *              RTOS_FALSE      If not
 ************************************************************************************************************************
 */

BOOLEAN RTOSTmrStop(RTOS_TMR *ptmr, RTOS_TMR_STOP_OPT opt, void *callback_arg, RTOS_TMR_PERR *perr){
    if(opt == RTOS_TMR_OPT_CALLBACK_ARG){
        ptmr->RTOSTmrCallback(callback_arg);
    }
    else if(opt == RTOS_TMR_OPT_CALLBACK){
        ptmr->RTOSTmrCallback(ptmr->RTOSTmrCallbackArg);
    }
    else if(opt == RTOS_TMR_OPT_NONE){
        //do nothing
    }
    else{
        //something is wrong
        //break and error message
        printf("something went wrong");
    }

    ptmr->RTOSTmrState = RTOS_TMR_STATE_STOPPED;
    return RTOS_TRUE;
}

/*
 ************************************************************************************************************************
 *                                      SIGNAL THAT IT'S TIME TO UPDATE THE TIMERS
 *
 * Description: This function is typically called by the ISR that occurs at the timer tick rate and is used to signal to
 *              RTOSTmr_Task() that it's time to update the timers.
 *
 * Arguments  : none
 *
 * Returns    : RTOS_ERR_NONE         The call was successful and the timer task was signaled.
 *
 ************************************************************************************************************************
 */

RTOS_TMR_PERR RTOSTmrSignal(void){
    //not entirely sure what to do
    //signal timet task to run through timers and update
    return RTOS_ERR_NONE;
}
