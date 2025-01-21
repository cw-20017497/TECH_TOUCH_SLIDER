#define CONFIG_UART_0   1
#define CONFIG_UART_1   1
#define CONFIG_UART_2   0
#define CONFIG_UART_3   0  

#if CONFIG_UART_0 
#pragma interrupt INTST0    r_uart0_interrupt_send
#pragma interrupt INTSR0    r_uart0_interrupt_receive
#endif

#if CONFIG_UART_1
#pragma interrupt INTST1    r_uart1_interrupt_send
#pragma interrupt INTSR1    r_uart1_interrupt_receive
#endif

#if CONFIG_UART_2 
#pragma interrupt INTST2    r_uart2_interrupt_send
#pragma interrupt INTSR2    r_uart2_interrupt_receive
#endif

#if CONFIG_UART_3 
#pragma interrupt INTST3    r_uart3_interrupt_send
#pragma interrupt INTSR3    r_uart3_interrupt_receive
#endif


#include "hw.h"
#include "hal_serial.h"
#include "timer.h"
#include <string.h>


#if CONFIG_UART_0
#define  UART_0_TXD TXD0
#define  UART_0_RXD RXD0
#endif

#if CONFIG_UART_1
#define  UART_1_TXD TXD1
#define  UART_1_RXD RXD1
#endif

#if CONFIG_UART_2
#define  UART_2_TXD TXD2
#define  UART_2_RXD RXD2
#endif

#if CONFIG_UART_3
#define  UART_3_TXD TXD3
#define  UART_3_RXD RXD3
#endif


#define MAX_UART_0_RX_BUF_SZ      30
#define MAX_UART_0_TX_BUF_SZ      30

#define MAX_UART_1_RX_BUF_SZ      128 
#define MAX_UART_1_TX_BUF_SZ      128 

#define MAX_UART_2_RX_BUF_SZ      128
#define MAX_UART_2_TX_BUF_SZ      128

#define MAX_UART_3_RX_BUF_SZ      255
#define MAX_UART_3_TX_BUF_SZ      255




// @ms
#define UART_0_RX_TIME_STAMP   10
#define UART_1_RX_TIME_STAMP   0
#define UART_2_RX_TIME_STAMP   10
#define UART_3_RX_TIME_STAMP   10


#if CONFIG_UART_0
U8 recv_0_buf[ MAX_UART_0_RX_BUF_SZ ];
U8 send_0_buf[ MAX_UART_0_TX_BUF_SZ ];
#else
U8 recv_0_buf[ 1 ];
U8 send_0_buf[ 1 ];
#endif

#if CONFIG_UART_1
U8 recv_1_buf[ MAX_UART_1_RX_BUF_SZ ];
U8 send_1_buf[ MAX_UART_1_TX_BUF_SZ ];
#else
U8 recv_1_buf[ 1 ];
U8 send_1_buf[ 1 ];
#endif

#if CONFIG_UART_2
U8 recv_2_buf[ MAX_UART_2_RX_BUF_SZ ];
U8 send_2_buf[ MAX_UART_2_TX_BUF_SZ ];
#else
U8 recv_2_buf[ 1 ];
U8 send_2_buf[ 1 ];
#endif

#if CONFIG_UART_3
U8 recv_3_buf[ MAX_UART_3_RX_BUF_SZ ];
U8 send_3_buf[ MAX_UART_3_TX_BUF_SZ ];
#else
U8 recv_3_buf[ 1 ];
U8 send_3_buf[ 1 ];
#endif

typedef struct _comm_
{
    I16 recv_len;
    I16 send_len;
    I16 tx_len;

    volatile I16 head;
    volatile I16 tail;
} comm_t;

comm_t  comm[ MAX_UART_ID ];



void    HAL_InitCommId( U8 uart_id )
{
    if( uart_id == UART_ID_0 )
    {
#if CONFIG_UART_0
        MEMSET( (void __FAR *)&recv_0_buf[0], 0, MAX_UART_0_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_0_buf[0], 0, MAX_UART_0_TX_BUF_SZ );
#endif
    }
    else if( uart_id == UART_ID_1 )
    {
#if CONFIG_UART_1
        MEMSET( (void __FAR *)&recv_1_buf[0], 0, MAX_UART_1_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_1_buf[0], 0, MAX_UART_1_TX_BUF_SZ );
#endif
    }
    else if( uart_id == UART_ID_2)
    {
#if CONFIG_UART_2
        MEMSET( (void __FAR *)&recv_2_buf[0], 0, MAX_UART_2_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_2_buf[0], 0, MAX_UART_2_TX_BUF_SZ );
#endif
    }
    else if( uart_id == UART_ID_3)
    {
#if CONFIG_UART_3
        MEMSET( (void __FAR *)&recv_3_buf[0], 0, MAX_UART_3_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_3_buf[0], 0, MAX_UART_3_TX_BUF_SZ );
#endif
    }

    MEMSET( (void __FAR *)&comm[ uart_id ], 0, sizeof( comm_t ) );
}

void    HAL_InitComm( void )
{
#if CONFIG_UART_0
    HAL_InitCommId( UART_ID_0 );
    R_UART0_Start();
#endif

#if CONFIG_UART_1
    HAL_InitCommId( UART_ID_1 );
    R_UART1_Start();
#endif

#if CONFIG_UART_2
    HAL_InitCommId( UART_ID_2 );
    R_UART2_Start();
#endif

#if CONFIG_UART_3
    HAL_InitCommId( UART_ID_3 );
    R_UART3_Start();
#endif

}

static I16 HAL_GetMaxRecvBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_UART_0_RX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_UART_1_RX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_UART_2_RX_BUF_SZ;
    }

    return MAX_UART_3_RX_BUF_SZ;
}

U8  HAL_IsFullRecvBuffer( U8 uart_id )
{
#if 0
    if( comm[ uart_id ].recv_len >= HAL_GetMaxRecvBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
#else
    I16 buf_size;

    buf_size = HAL_GetMaxRecvBufferLength( uart_id );
    if( ((comm[uart_id].head + 1) % buf_size) == comm[uart_id].tail)
    {
        return TRUE;
    }
        
    return FALSE;
#endif
}

U8  HAL_IsEmptyRecvBuffer( U8 uart_id )
{
#if 0
    if( comm[ uart_id ].recv_len > 0 )
    {
        return FALSE;
    }

    return TRUE;
#endif
    if( comm[uart_id].head == comm[uart_id].tail)
    {
        return TRUE;
    }
        
    return FALSE;

}

static I16 HAL_GetMaxSendBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_UART_0_TX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_UART_1_TX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_UART_2_TX_BUF_SZ;
    }

    return MAX_UART_3_TX_BUF_SZ;
}

U8  HAL_IsFullSendBuffer( U8 uart_id )
{
    if( comm[ uart_id ].send_len >= HAL_GetMaxSendBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
}

static U8   IsCompleteTx( U8 uart_id );
U8  HAL_IsEmptySendBuffer( U8 uart_id )
{
    return IsCompleteTx( uart_id );
}

void HAL_InitRecvLength(U8 uart_id )
{
    comm[ uart_id ].recv_len = 0;
}

I16  HAL_GetRecvLength( U8 uart_id )
{
    return comm[ uart_id ].recv_len;
}

I16  HAL_GetSendLength( U8 uart_id )
{
    return comm[ uart_id].send_len;
}


void    HAL_SetRecvBuffer(U8 uart_id, U8 _byte )
{
#if 0
    comm_t *p_comm;

    p_comm = &comm[ uart_id ];
    if( uart_id == UART_ID_0 )
    {
        recv_0_buf[ p_comm->recv_len ] = _byte;
    }
    else if( uart_id == UART_ID_1 )
    {
        recv_1_buf[ p_comm->recv_len ] = _byte;
    }
    else if( uart_id == UART_ID_2)
    {
        recv_2_buf[ p_comm->recv_len ] = _byte;
    }
    else //if( uart_id == UART_ID_3)
    {
        recv_3_buf[ p_comm->recv_len ] = _byte;
    }
    p_comm->recv_len++;
#else
    comm_t *p_comm;
    I16 buf_len;

    p_comm = &comm[ uart_id ];
    if( uart_id == UART_ID_0 )
    {
        recv_0_buf[ p_comm->head ] = _byte;
    }
    else if( uart_id == UART_ID_1 )
    {
        recv_1_buf[ p_comm->head ] = _byte;
    }
    else if( uart_id == UART_ID_2)
    {
        recv_2_buf[ p_comm->head ] = _byte;
    }
    else //if( uart_id == UART_ID_3)
    {
        recv_3_buf[ p_comm->head ] = _byte;
    }

    buf_len = HAL_GetMaxSendBufferLength( uart_id );
    p_comm->head = (p_comm->head + 1) % buf_len;
#endif

}

U8 HAL_GetRecvBuffer( U8 uart_id )
{
#if 0
    if( uart_id == UART_ID_0 )
    {
        return recv_0_buf[ index ];
    }
    else if( uart_id == UART_ID_1 )
    {
        return recv_1_buf[ index ];
    }
    else if( uart_id == UART_ID_2)
    {
        return recv_2_buf[ index ];
    }
    else //if( uart_id == UART_ID_3)
    {
        return recv_3_buf[ index ];
    }

#else
    comm_t *p_comm;
    I16 buf_len;
    U8 _byte = 0;

    p_comm = &comm[ uart_id ];
    if( uart_id == UART_ID_0 )
    {
        _byte = recv_0_buf[ p_comm->tail ];
    }
    else if( uart_id == UART_ID_1 )
    {
        _byte = recv_1_buf[ p_comm->tail ];
    }
    else if( uart_id == UART_ID_2)
    {
        _byte = recv_2_buf[ p_comm->tail ];
    }
    else //if( uart_id == UART_ID_3)
    {
        _byte = recv_3_buf[ p_comm->tail ];
    }

    buf_len = HAL_GetMaxSendBufferLength( uart_id );
    p_comm->tail = (p_comm->tail + 1) % buf_len;


    return _byte;
#endif
}


void    HAL_SetSendBuffer( U8 uart_id, U8 _byte )
{
    comm_t *p_comm;

    if( HAL_IsFullSendBuffer( uart_id ) == FALSE )
    {
        p_comm = &comm[ uart_id ];
        if( uart_id == UART_ID_0 )
        {
            send_0_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == UART_ID_1 )
        {
            send_1_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == UART_ID_2)
        {
            send_2_buf[ p_comm->send_len ] = _byte;
        }
        else //if( uart_id == UART_ID_3)
        {
            send_3_buf[ p_comm->send_len ] = _byte;
        }
        p_comm->send_len++;
    }
}

U8  HAL_GetSendBuffer( U8 uart_id, U16 index )
{
    if( uart_id == UART_ID_0 )
    {
        return send_0_buf[index];
    }
    else if( uart_id == UART_ID_1 )
    {
        return send_1_buf[index];
    }
    else if( uart_id == UART_ID_2)
    {
        return send_2_buf[index];
    }
    else //if( uart_id == UART_ID_3)
    {
        return send_3_buf[index];
    }
}


void    HAL_SendByte( U8 uart_id )
{
    U8 ch;
    comm_t *p_comm;


    p_comm  = &comm[ uart_id ];
    if( uart_id == UART_ID_0 )
    {
        ch = send_0_buf[p_comm->tx_len];
    }
    else if( uart_id == UART_ID_1 )
    {
        ch = send_1_buf[p_comm->tx_len];
    }
    else if( uart_id == UART_ID_2)
    {
        ch = send_2_buf[p_comm->tx_len];
    }
    else if( uart_id == UART_ID_3)
    {
        ch = send_3_buf[p_comm->tx_len];
    }

    p_comm->tx_len++;

    if( uart_id == UART_ID_0 )
    {
#if CONFIG_UART_0
        UART_0_TXD = ch;
#endif
    }
    else if( uart_id == UART_ID_1 )
    {
#if CONFIG_UART_1
        UART_1_TXD = ch;
#endif
    }
    else if( uart_id == UART_ID_2 )
    {
#if CONFIG_UART_2
        UART_2_TXD = ch;
#endif
    }
    else if( uart_id == UART_ID_3 )
    {
#if CONFIG_UART_3
        UART_3_TXD = ch;
#endif
    }
}

static U8   IsCompleteTx( U8 uart_id )
{
    if( comm[uart_id].tx_len < comm[ uart_id ].send_len )
    {
        return FALSE;
    }

    return TRUE;

}

/* INTERRUPT */

/* UART 0 */
#if CONFIG_UART_0
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;


    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    rx_data = UART_0_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( UART_ID_0 ) == FALSE )
        {
            HAL_SetRecvBuffer( UART_ID_0, rx_data );
        }
        else
        {
            HAL_InitCommId( UART_ID_0 );
        }

        StartTimer( TIMER_ID_UART_0_RX, UART_0_RX_TIME_STAMP );
    }
}

__interrupt static void r_uart0_interrupt_send(void)
{
    if( IsCompleteTx( UART_ID_0 ) == FALSE )
    {
        HAL_SendByte( UART_ID_0 );
    }
    else
    {
        HAL_InitCommId( UART_ID_0 );
    }
}
#endif

/* UART 1 */
#if CONFIG_UART_1
__interrupt static void r_uart1_interrupt_receive(void)
{
    volatile U8 rx_data;
    volatile U8 err_type;
    static U8 cnt = 0;

    err_type = (U8)(SSR03 & 0x0007U);
    SIR03 = (U16)err_type;

    rx_data = UART_1_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( UART_ID_1 ) == FALSE )
        {
            HAL_SetRecvBuffer( UART_ID_1, rx_data );
        }
        else
        {
            HAL_InitCommId( UART_ID_1 );
        }
        
        StartTimer( TIMER_ID_UART_1_RX, 10 );
        if( rx_data == 0x55 )
        {
            cnt++;
            if( cnt >=3 )
            {
                StartTimer( TIMER_ID_UART_1_RX, 3 );
                StartTimer( TIMER_ID_UART_1_RX_DONE, UART_1_RX_TIME_STAMP );
                cnt = 0;
            }
        }
        else
        {
            cnt = 0;
        }
    }
}

__interrupt static void r_uart1_interrupt_send(void)
{
    if( IsCompleteTx( UART_ID_1 ) == FALSE )
    {
        HAL_SendByte( UART_ID_1 );
        StartTimer( TIMER_ID_UART_1_TX_DONE, 10 );
    }
    else
    {
        HAL_InitCommId( UART_ID_1 );
    }
}
#endif

/* UART 2 */
#if CONFIG_UART_2
U16 the_rx_err_count = 0;
__interrupt static void r_uart2_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR11 & 0x0007U);
    SIR11 = (uint16_t)err_type;

    rx_data = UART_2_RXD;

    EI();
    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( UART_ID_2 ) == FALSE )
        {
            HAL_SetRecvBuffer( UART_ID_2, rx_data );
        }
        else
        {
            HAL_InitCommId( UART_ID_2 );
        }

        StartTimer( TIMER_ID_UART_2_RX, UART_2_RX_TIME_STAMP );
    }
    else
    {
        the_rx_err_count++;
    }
}

__interrupt static void r_uart2_interrupt_send(void)
{
    EI();
    if( IsCompleteTx( UART_ID_2 ) == FALSE )
    {
        HAL_SendByte( UART_ID_2 );
        StartTimer( TIMER_ID_UART_2_TX_DONE, 10 );
    }
    else
    {
        HAL_InitCommId( UART_ID_2 );
    }

}
#endif


/* UART 3 */
#if CONFIG_UART_3
U16 the_isr_err_count = 0;
__interrupt static void r_uart3_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR13 & 0x0007U);
    SIR13 = (uint16_t)err_type;

    rx_data = UART_3_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( UART_ID_3 ) == FALSE )
        {
            HAL_SetRecvBuffer( UART_ID_3, rx_data );
        }
        else
        {
            HAL_InitCommId( UART_ID_3 );
        }

        StartTimer( TIMER_ID_UART_3_RX, UART_3_RX_TIME_STAMP );
    }
    else
    {
        the_isr_err_count++;
    }
}


__interrupt static void r_uart3_interrupt_send(void)
{
    if( IsCompleteTx( UART_ID_3 ) == FALSE )
    {
        HAL_SendByte( UART_ID_3 );
    }
    else
    {
        HAL_InitCommId( UART_ID_3 );
    }
}
#endif
