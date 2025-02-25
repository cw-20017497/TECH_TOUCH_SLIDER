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

#define STX_1                 0xFF
#define STX_2                 0x99

#define MIN_PKT_SZ            8


static U8   check_crc( U8 *buf, I16 len )
{
    U8 i;
    U8 checksum = 0;
    U8 sum = 0;

    len = len - 1;
    checksum = buf[len];

    // checksum - sum byte 3 ~ 7 
    for( i = 2; i < len; i++ )
    {
        sum += buf[i];
    }

    if( checksum != sum )
    {
        return FALSE;
    }

    return TRUE;
}


#define START_READ_STX_1        0
#define START_READ_STX_2        1
#define START_READ_PAYLOAD      2
I16 ReadPacket_Key( U8 id , U8 *recv_pkt )
{
    U16  i = 0;
    I16 len = 0;
    U8 buf;
    U8 startRead = START_READ_STX_1;
    static U8 pkt_count = 0;


    while( HAL_IsEmptyRecvBuffer( id ) == FALSE )
    {
        buf = HAL_GetRecvBuffer( id );
        // check stx 1
        if( startRead == START_READ_STX_1 && buf == STX_1 )
        {
            startRead = START_READ_STX_2;
            recv_pkt[ len++ ] = buf;
        }
        // check stx 2
        else if( startRead == START_READ_STX_2 && buf == STX_2 )
        {
            startRead = START_READ_PAYLOAD;
            recv_pkt[ len++ ] = buf;
        }
        else if( startRead == START_READ_PAYLOAD )
        {
            recv_pkt[ len++ ] = buf;

            pkt_count++;
            if(pkt_count >= MIN_PKT_SZ - 2/*except STX 2byte*/ )
            {
                pkt_count = 0;
                break;
            }
        }
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

    if( check_crc( buf, len ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}



static I16 ParserReqLed(U8 *buf);
I16 ParserPkt_Key( U8 *buf, I16 len)
{
    len = ParserReqLed(&buf[2]);

    return len;
}


I16 Crc16_Key( U8 *buf, I16 len )
{
    return 0;
}



static I16 ParserReqLed(U8 *buf)
{
    HAL_SetKeyVal( buf[0] );
    HAL_SetSlider( &buf[1] );

    SetCommQueueMain( PKT_REQ_KEY );
    return TRUE;
}

I16 MakePkt_Key( CommHeader_T *p_comm, U8 *buf )
{
    return 0;
}



