#include <stdio.h>
#include <string.h>
#include "prj_type.h"
#include "timer.h"
#include "hal_serial.h"
#include "hal_led.h"
#include "buzzer.h"
#include "parser.h"
#include "parser_front.h"
#include "process_display.h"
#include "process_sys_event.h"


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
static void Evt_1sec_Handler( void );
static void Evt_Front_Handler( void );



const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                    NORMAL,                     FCT,        EOL */
    { TIMER_ID_1MS,                 Evt_1ms_Handler,            NULL,       NULL },
    { TIMER_ID_10MS,                Evt_10ms_Handler,           NULL,       NULL },
    { TIMER_ID_100MS,               Evt_100ms_Handler,          NULL,       NULL },
    { TIMER_ID_1SEC,                Evt_1sec_Handler,           NULL,       NULL },
    { TIMER_ID_FRONT,               Evt_Front_Handler,          NULL,       NULL },
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
    ProcessDisplay();
}


static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);


}


static void Evt_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );
}



// 50ms 주기로 FRONT COMMUNICATION
#define FRONT_REFRESH_TIME     50 // @50ms...
static void Evt_Front_Handler( void )
{
    StartTimer( TIMER_ID_FRONT, FRONT_REFRESH_TIME );

//    if( HAL_IsUpdateLed() == TRUE )
//    {
//        SetCommHeader( COMM_ID_FRONT, PKT_FRONT_REQ_LED );
//        StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );
//
//        HAL_UpdateLed();
//    }
}



void TimerIsrCallback(void)
{
    BuzControl();
}

void InitSystem(void)
{
    /* IO */
    HAL_InitComm();
    InitBuzzer();

    HAL_InitLed();
    InitDisplay();

    RegisterTimerISR( TimerIsrCallback  );
}

