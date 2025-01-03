#include "comm_queue.h"
#include "queue.h"
#include "timer.h"
#include "timer_id.h"

#include "parser_main.h"

#define MAX_QUEUE_NUM   30
CommData_T comm_main[ MAX_QUEUE_NUM ];
SQueue_T comm_queue[ MAX_COMM_ID ];     


#define RETRY_REQ_COUNT    5
#define RETRY_ACK_COUNT    1
#define RETRY_WAIT_TIME   80
CommData_T data;

void InitCommQueue(void)
{
    InitQueue( &comm_queue[ COMM_ID_MAIN ], &comm_main, sizeof( CommData_T ), MAX_QUEUE_NUM );
}


void SetCommQueueMain(U8 packet)
{
    CommData_T data;

    data.packet         = packet;

    if( packet == PKT_REQ_KEY )
    {
        data.retry_count = RETRY_REQ_COUNT;
        EnQueue( &comm_queue[ COMM_ID_MAIN ], &data);
    }
    else
    {
        data.retry_count = RETRY_ACK_COUNT;
        EnQueueFront( &comm_queue[ COMM_ID_MAIN ], &data);
    }

}


#if 0
U8 dbg_err_retry_count = 0;
void SendPacketQueueMain(void)
{
    if( IsExpiredTimer( TIMER_ID_COMM_WAIT_1 ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( IsExpiredTimer( TIMER_ID_UART_1_TX_DONE ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( data.retry_count == 0 )
    {
        if( DeQueue( &comm_queue[ COMM_ID_MAIN ], (void *)&data ) == TRUE )
        {
            // decrement count -> send message 
            data.retry_count--;

            SetCommHeader( COMM_ID_MAIN, data.packet);
            StartTimer( TIMER_ID_COMM_MAIN_TX, 0 );
            StartTimer( TIMER_ID_COMM_WAIT_1, RETRY_WAIT_TIME );
        }
    }
    else
    {
        dbg_err_retry_count++;
        data.retry_count--;

        SetCommHeader( COMM_ID_MAIN, data.packet);
        StartTimer( TIMER_ID_COMM_MAIN_TX, 0 );
        StartTimer( TIMER_ID_COMM_WAIT_1, RETRY_WAIT_TIME );
    }

}
#else
void SendPacketQueueMain(void)
{
    if( IsExpiredTimer( TIMER_ID_COMM_WAIT_1 ) != TIMER_EXPIRE 
        || IsExpiredTimer( TIMER_ID_UART_1_TX_DONE ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( data.retry_count == 0 )
    {
        if( DeQueue( &comm_queue[ COMM_ID_MAIN ], (void *)&data ) == FALSE )
        {
            return ;
        }
    }

    data.retry_count--;

    SetCommHeader( COMM_ID_MAIN, data.packet);
    StartTimer( TIMER_ID_COMM_MAIN_TX, 0 );

    if( data.retry_count != 0 )
    {
        StartTimer( TIMER_ID_COMM_WAIT_1, RETRY_WAIT_TIME );
    }
}
#endif


U8 dbg_retry_ok_count = 0;      // for dbg
U8 dbg_retry_ng_count = 0;      // for dbg
void ReceivedMainAck(U8 packet)
{
    if( packet == data.packet )
    {
        data.retry_count = 0;
	  StartTimer( TIMER_ID_COMM_WAIT_1, 0 );
        dbg_retry_ok_count++;

    }
    else
    {
        dbg_retry_ng_count++;
    }
}


