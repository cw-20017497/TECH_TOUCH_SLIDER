#include "comm_queue.h"
#include "queue.h"

#include "timer.h"
#include "timer_id.h"

#define MAX_QUEUE_NUM   100
CommData_T comm_front[ MAX_QUEUE_NUM ];
SQueue_T comm_queue[ MAX_COMM_ID ];     


#define RETRY_REQ_COUNT     3//5
#define RETRY_ACK_COUNT     1
#define RETRY_WAIT_TIME     20//100
CommData_T data;

void InitCommQueue(void)
{
    InitQueue( &comm_queue[ COMM_ID_FRONT ], &comm_front, sizeof( CommData_T ), MAX_QUEUE_NUM );
}


void SetCommQueueFront(U8 packet)
{
    CommData_T data;

    data.packet         = packet;

    if( packet == PKT_FRONT_ACK_LED )
    {
        data.retry_count = RETRY_ACK_COUNT;
        EnQueueFront( &comm_queue[ COMM_ID_FRONT ], &data);
    }
    else
    {
        data.retry_count = RETRY_REQ_COUNT;
        EnQueue( &comm_queue[ COMM_ID_FRONT ], &data);
    }

}


U32 dbg_err_retry_count[ RETRY_REQ_COUNT ] = {0};    // for dbg
void SendPacketQueueFront(void)
{
    if( IsExpiredTimer( TIMER_ID_QUEUE_RETRY_WAIT ) != TIMER_EXPIRE 
        || IsExpiredTimer( TIMER_ID_UART_2_RX ) != TIMER_EXPIRE 
        || IsExpiredTimer( TIMER_ID_UART_2_TX_DONE ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( data.retry_count == 0 )
    {
        if( DeQueue( &comm_queue[ COMM_ID_FRONT ], (void *)&data ) == FALSE )
        {
            return ;
        }
    }

    data.retry_count--;

    SetCommHeader( COMM_ID_FRONT, data.packet);
    StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );

    if( data.retry_count != 0 )
    {
        StartTimer( TIMER_ID_QUEUE_RETRY_WAIT, RETRY_WAIT_TIME );
    }

    ////////////////////////////////////////////////////
    // for dbg...
    if( (data.packet & 0x80) != 0x80 )
    {
        if( data.retry_count < RETRY_REQ_COUNT )
        {
            dbg_err_retry_count[ data.retry_count ]++;
        }
    }
    ////////////////////////////////////////////////////
}


U8 dbg_retry_ok_count = 0;      // for dbg
U8 dbg_retry_ng_count = 0;      // for dbg
void ReceivedFrontAck(U8 packet)
{
    if( packet == data.packet )
    {
        data.retry_count = 0;
        dbg_retry_ok_count++;

        StartTimer( TIMER_ID_QUEUE_RETRY_WAIT, 0 );
    }
    else
    {
        dbg_retry_ng_count++;
    }
}


