
#include "r_cg_timer.h"

#include "prj_type.h"
#include "timer.h"
#include "process_sys_event.h"

#include "hal_serial.h"
#include "hal_led.h"
#include "hal_key.h"
#include "comm_queue.h"


#include <stdio.h>
#include <string.h>


typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_100ms_Handler( void );

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                 NORMAL,                 FCT,    EOL */
    { TIMER_ID_1MS,              Evt_1ms_Handler,        NULL,   NULL },
    { TIMER_ID_10MS,             Evt_10ms_Handler,       NULL,   NULL },
    { TIMER_ID_100MS,            Evt_100ms_Handler,      NULL,   NULL },
};
#define	SZ_LIST		( sizeof( SysEventList ) / sizeof( SysEvent_T ) )

void ProcessEventHandler( void )
{
    U8 i;
    Action_T fun;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( IsExpiredTimer( SysEventList[ i ].timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( SysEventList[ i ].timerId );

            // Get Normal Mode Event Handler
            fun = SysEventList[ i ].pFun;

            // Call a event handl
            if( fun != NULL )
            {
                fun();
            }
        }
    }
}

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

}

static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    
}


static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);
}


void InitSystem(void)
{
    /* IO */
    HAL_InitComm();
   
    InitCommQueue();

    R_TAU0_Channel1_Start();   // 100us dimming timer interrupt
}

