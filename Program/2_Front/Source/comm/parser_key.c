/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_key.h"
#include "util.h"
#include "timer.h"

#include "hal_led.h"
#include "hal_key.h"

#include "parser_main.h"
#include "comm_queue.h"


/***********************************************************************************************
 * DEFINITION 
 */

//#define STX                 0xAA
//#define ETX                 0x55

//#define PKT_REQ_LED         0x01
//#define PKT_ACK_LED         (0x80|PKT_REQ_LED)

//#define PKT_REQ_KEY         0x10
//#define PKT_ACK_KEY         (PKT_ACK|PKT_REQ_KEY)

#define MIN_PKT_SZ          5

static U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
{
    U8 i = 0;
    U16 wCRCin = 0x0000;
    U16 wCPoly = 0x1021;
    U8 wChar = 0;

    while(usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= ((U16)wChar << 8);
        for(i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }

    return (wCRCin);
}

static U8   check_crc( U8 *buf, I16 len )
{
    U16 crc16 = 0;

    crc16 = ( ( (U16)buf[ len - 3 ] ) << 8 ) & 0xFF00;
    crc16 |=    (U16)( buf[ len - 2 ] );

    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 ReadPacket_Key( U8 id , U8 *recv_pkt )
{
    U16  i = 0;
    I16 len = 0;

    while( HAL_IsEmptyRecvBuffer( id ) == FALSE )
    {
        recv_pkt[ i++ ] = HAL_GetRecvBuffer( id );
        len++;
    }

    return len; /* RECEIVED BUF SIZE */
}

I16 IsValidPkt_Key( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    //if( check_crc( buf, len ) == FALSE )
    //{
    //    return FALSE;
    //}

    return TRUE;
}



static I16 ParserReqLed(U8 *buf);
I16 ParserPkt_Key( U8 *buf, I16 len)
{
    len = ParserReqLed(&buf[0]);

    return len;
}

I16 Crc16_Key( U8 *buf, I16 len )
{
    U16 mu16Chksum = 0;


    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu16Chksum = Rx_CRC_CCITT( buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_HIGH_BYTE(mu16Chksum);
    buf[ len - 2 ] = GET_LOW_BYTE(mu16Chksum);

    return len;
}


// Paylode
// [0] : 
U8 dbg_recv_key[5];
static I16 ParserReqLed(U8 *buf)
{
    HAL_SetKeyVal( buf[0] );
    HAL_SetSlider( &buf[1] );


    //SetCommHeader( COMM_ID_MAIN, PKT_REQ_KEY );
    //StartTimer( TIMER_ID_COMM_MAIN_TX, 0 );
    SetCommQueueMain( PKT_REQ_KEY );


    return TRUE;
}

// [0] : button 
// [1] : slider 1 flick -100 ~ +100
// [2] : slider 1 position
// [3] : slider 2 flick -100 ~ +100
// [4] : slider 2 position
I16 MakePkt_Key( CommHeader_T *p_comm, U8 *buf )
{
    I16	len	= -1;

    return len;
}



