#include "hw.h"
#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "sound.h"
#include "parser.h"
#include "process_sys_event.h"

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_100MS, 100);
    StartTimer( TIMER_ID_1SEC,  SEC(1));

    StartTimer( TIMER_ID_QUEUE_RETRY_WAIT, 1);
    StartTimer( TIMER_ID_UART_2_RX, 1 );
    StartTimer( TIMER_ID_UART_2_TX_DONE, 1);
}


void main( void )
{
    Delay_MS( 500 );

    R_WDT_Restart();

    InitSystem();
    InitStartTimers();
    EI();

    /* EEPROM */
    R_WDT_Restart();
 

    Sound( SOUND_POWER_ON );
    while(1)
    {
        R_WDT_Restart();

        ProcessEventHandler();

        /* COMM */
        RecvPacketHandler();
        SendPacketHandler();
    }
}
