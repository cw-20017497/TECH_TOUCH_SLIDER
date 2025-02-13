/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_front.h"
#include "util.h"
#include "timer.h"
#include "hal_led.h"
#include "hal_key.h"
#include "comm_queue.h"

#include "hal_slider.h"


/***********************************************************************************************
 * DEFINITION 
 */

#define STX         0xAA
#define ETX         0x55

#define PKT_ACK             0x80

#define PKT_REQ_LED         0x01
#define PKT_ACK_LED         (PKT_ACK|PKT_REQ_LED)

#define PKT_REQ_KEY         0x10
#define PKT_ACK_KEY         (PKT_ACK|PKT_REQ_KEY)

#define MIN_PKT_SZ          3   /* STX, ETX, CHECKSUM */

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
    U16 crc16_cal = 0;

    crc16 = GET_UINT_WORD( buf[len-4], buf[len-5] );
    crc16_cal = Rx_CRC_CCITT( buf, (U16)( len - 5 ) );
    if( crc16 != crc16_cal )
    {
        return FALSE;
    }

    return TRUE;
}


U8 dbg_read_heade_err_index = 0;
U32 dbg_read_head_err[20] = {0};

I16 ReadPacket_Front( U8 id , U8 *recv_pkt )
{
    U16  i = 0;
    I16 len = 0;
    U8 buf;
    U8 startRead = FALSE;
    static U8 etx_count = 0;

    while( HAL_IsEmptyRecvBuffer( id ) == FALSE )
    {
        buf = HAL_GetRecvBuffer( id );
        if( startRead == FALSE &&
                buf == STX )
        {
            startRead = TRUE;
            etx_count = 0;
            recv_pkt[ len++ ] = buf;
        }
        else if( startRead == TRUE )
        {
            recv_pkt[ len++ ] = buf;
            if( buf != ETX )
            {
                etx_count = 0;
            }
            else
            {
                etx_count++;
                if(etx_count >= 3 )
                {
                    etx_count = 0;
                    break;
                }
            }
        }
        else
        {
            ///////////////////////////////////////////
            // debug..
            ///////////////////////////////////////////
            //dbg_read_head_err[ dbg_read_heade_err_index ]++;
            ///////////////////////////////////////////
        }
    }

    if( etx_count != 3 )
    {
        dbg_read_head_err[ dbg_read_heade_err_index ] = len;
        if( dbg_read_heade_err_index < 20 )
        {
            dbg_read_heade_err_index++;
        }
    }


    return len; /* RECEIVED BUF SIZE */

}



U8 dbg_valid_err_1 = 0;
U8 dbg_valid_err_2 = 0;
U8 dbg_valid_err_3 = 0;
U8 dbg_valid_msg[5][40];
U8 dbg_valid_ok_msg[5][40];

I16 IsValidPkt_Front( U8 *buf, I16 len )
{
    static U8 i = 0;
    static U8 j = 0;


    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        dbg_valid_err_1++;
        return FALSE;
    }

    if( check_crc( buf, len ) == FALSE )
    {
        dbg_valid_err_2++;
        memcpy( &dbg_valid_msg[i++][0], buf, len );
        if( i >= 4 ) i = 0;
        return FALSE;
    }
    else
    {

        dbg_valid_ok_msg[j][0] = (U8)len;
        memcpy( &dbg_valid_ok_msg[j++][1], buf, len );
        if( j >= 4 ) j = 0;
    }

    return TRUE;
}


static I16 ParserReqKey(U8 *buf);
static I16 ParserAckLed(U8 *buf);

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t Parser;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { PKT_REQ_KEY,          ParserReqKey },
    { PKT_FRONT_ACK_LED,    ParserAckLed },
};

#define SZ_PS_TABLE ( sizeof( parser_list ) / sizeof( parser_list_t ))

I16 ParserPkt_Front( U8 *buf, I16 len)
{
    U8 mu8Type;
    action_t	pParser;
    U8 i;


    for( i = 0; i < SZ_PS_TABLE; i++ )
    {
        mu8Type = parser_list[ i ].Type;
        pParser = parser_list[ i ].Parser;

        if( mu8Type == buf[1] )
        {
            if( pParser != NULL )
            {
                len = pParser( &buf[2] );
            }
            break;
        }
    }

    return len;
}

I16 Crc16_Front( U8 *buf, I16 len )
{
    U16 mu16Chksum = 0;
    

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu16Chksum = Rx_CRC_CCITT( buf, (U16)(len - 5));
    buf[ len - 5 ] = GET_HIGH_BYTE(mu16Chksum);
    buf[ len - 4 ] = GET_LOW_BYTE(mu16Chksum);

    return len;
}



U8 slider[4];
static I16 ParserReqKey(U8 *buf)
{
    U32 key;

    key = GET_UINT_WORD_32( buf[0], buf[1], buf[2], buf[3] );
    HAL_SetKeyVal( key );

    slider[0] = buf[4];
    slider[1] = buf[5];
    slider[2] = buf[6];
    slider[3] = buf[7];

    HAL_SetSliderVal( HAL_SLIDER_1, buf[5] );
    HAL_SetSliderVal( HAL_SLIDER_2, buf[7] );

    //SetSlideValue( SLIDER_CIRCLE, buf[5], FALSE );
    //SetSlideValue( SLIDER_BAR, buf[7], FALSE );

    //if( slider[1] == 0 )
    //{
    //    slider[1] = slider[3];
    //}

    // ACK 
    SetCommHeader( COMM_ID_FRONT, PKT_ACK_KEY );
    StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );


    return TRUE;
}

U8 dbg_led_ack_count = 0;
static I16 ParserAckLed(U8 *buf)
{
    ReceivedFrontAck( PKT_FRONT_REQ_LED );
    dbg_led_ack_count++;
    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktAckKey( U8 *buf );
static I16 MakePktReqLed( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_ACK_KEY,        MakePktAckKey },
    { PKT_FRONT_REQ_LED,  MakePktReqLed  },
};
#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_Front( CommHeader_T *p_comm, U8 *buf )
{
    U8 mu8Type;
    action_t	p_make_pkt;
    U8 i;
    I16	len	= -1;


    for( i = 0; i < SZ_TABLE; i++ )
    {
        mu8Type = make_list[ i ].Type;
        p_make_pkt = make_list[ i ].MakePkt;

        if( mu8Type == (U8)p_comm )
        {
            if( p_make_pkt != NULL )
            {
                len = p_make_pkt( buf );
            }
            break;
        }
    }

    return len;
}

U8 dbg_the_ack_key = 0;
static I16 MakePktAckKey( U8 *buf )
{
    I16 mi16Len = 0;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_ACK_KEY;

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;
    buf[ mi16Len++ ] = ETX;
    buf[ mi16Len++ ] = ETX;

    dbg_the_ack_key++;
    return mi16Len;
}

U8 dbg_the_req_led = 0;
static I16 MakePktReqLed( U8 *buf )
{
    I16 mi16Len = 0;
    U8  mu8Buf[MAX_LED_BUF];
    U8 i;

    dbg_the_req_led++;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_FRONT_REQ_LED;
    
    // LED 
    HAL_GetLedOnOffStatus( &mu8Buf[0] );
    for( i = 0; i < MAX_LED_BUF ; i++ )
    {
        buf[ mi16Len++ ] = mu8Buf[i];
    }

    // LED DIMMING
    HAL_GetLedDimmingStatus( &mu8Buf[0]);
    for( i = 0; i < MAX_LED_BUF ; i++ )
    {
        buf[ mi16Len++ ] = mu8Buf[i];
    }

    // LED DIMMING DUTY
    buf[ mi16Len++ ] = HAL_GetLedDimmingDuty();

    // LED DUTY
    buf[ mi16Len++ ] = HAL_GetLedDuty(); 

    // UNUSED
    buf[ mi16Len++ ] = 0;
    
    // UNUSED
    buf[ mi16Len++ ] = 0;

    // MODE
    buf[ mi16Len++ ] = 0;

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;
    buf[ mi16Len++ ] = ETX;
    buf[ mi16Len++ ] = ETX;
    return mi16Len;
}


