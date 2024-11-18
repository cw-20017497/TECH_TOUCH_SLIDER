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

    //crc16 = ( ( (U16)buf[ len - 1 ] ) << 8 ) & 0xFF00;
    //crc16 |=    ( buf[ len - 2 ] );
    crc16 = GET_UINT_WORD( buf[len-2], buf[len-3] );

    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPkt_Front( U8 *buf, I16 len )
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


static I16 ParserReqKey(U8 *buf);

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t Parser;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { PKT_REQ_KEY,  ParserReqKey },
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
                StartTimer( TIMER_ID_COMM_FRONT_RX_ERR, SEC(3));
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

    mu16Chksum = Rx_CRC_CCITT( buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_HIGH_BYTE(mu16Chksum);
    buf[ len - 2 ] = GET_LOW_BYTE(mu16Chksum);

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

    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktReqLed( U8 *buf );
const static make_list_t make_list[] = 
{
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


static I16 MakePktReqLed( U8 *buf )
{
    I16 mi16Len = 0;
    U8  mu8Buf[MAX_LED_BUF];
    U8 i;


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
    return mi16Len;
}


