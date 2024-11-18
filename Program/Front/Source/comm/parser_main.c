/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_main.h"
#include "util.h"
#include "timer.h"

#include "hal_led.h"
#include "hal_key.h"

/***********************************************************************************************
 * DEFINITION 
 */

#define STX                 0xAA
#define ETX                 0x55

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

    crc16 = ( ( (U16)buf[ len - 3 ] ) << 8 ) & 0xFF00;
    crc16 |=    (U16)( buf[ len - 2 ] );

    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPkt_Main( U8 *buf, I16 len )
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

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t Parser;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { PKT_REQ_LED,    ParserReqLed  },
};

#define SZ_PS_TABLE ( sizeof( parser_list ) / sizeof( parser_list_t ))

I16 ParserPkt_Main( U8 *buf, I16 len)
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

I16 Crc16_Main( U8 *buf, I16 len )
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
// 0~3: LED ONOFF
// 4~6: LED DIMMING
// 7 : LED DUTY
// 8 : UV ONOFF
// 9 : WIFI-PARING MODE
static I16 ParserReqLed(U8 *buf)
{
    U32 mu32Val;


    // LED 
    HAL_SetOnOffLED( &buf[0], MAX_LED );

    // DIMMING
#if CONFIG_DIMMING
    HAL_SetDimmingLED( &buf[6], MAX_LED );      // LED DIMMING ON/OFF
    HAL_SetDimmingDuty( buf[12] );               // LED DIMMING DUTY
    HAL_SetOnOffDuty( buf[13] );                 // LED LED DUTY
#else
    HAL_SetDimmingLED( 0UL );
#endif
    // UV
    //HAL_SetUvOnOffId( UV_WATER, buf[9] );

    // Send ACK 
    //SetCommHeader( COMM_ID_MAIN, PKT_ACK_LED );
    //StartTimer( TIMER_ID_COMM_MAIN_TX, 1 );
    return TRUE;
}




typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktReqKey( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_ACK_LED,           NULL  },
    { PKT_REQ_KEY,           MakePktReqKey  },
};
#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_Main( CommHeader_T *p_comm, U8 *buf )
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



// STX(1) + PKT_ACK_LED(1) + KEY(4) + INPUT(1) + ADC(8) + CRC(2) + EXT(1)
// SIZE : 18 bytes
static I16 MakePktReqKey( U8 *buf )
{
    I16 mi16Len = 0;
    U16 mu16Val = 0;
    U32 mu32Val = 0;
    U8 slider[4];


    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_REQ_KEY;

    // KEY 
    mu32Val = HAL_GetKeyVal();
    buf[ mi16Len++ ] = GET_32_BYTE_32( mu32Val );
    buf[ mi16Len++ ] = GET_32_BYTE_24( mu32Val );
    buf[ mi16Len++ ] = GET_32_BYTE_16( mu32Val );
    buf[ mi16Len++ ] = GET_32_BYTE_8( mu32Val );

    HAL_GetSlider( &slider[0] );
    buf[ mi16Len++ ] = slider[0];
    buf[ mi16Len++ ] = slider[1];
    buf[ mi16Len++ ] = slider[2];
    buf[ mi16Len++ ] = slider[3];

    // INPUT 
    buf[ mi16Len++ ] = 0;
    

    buf[ mi16Len++ ] = 0;   // UNUSED
    buf[ mi16Len++ ] = 0;   // UNUSED
    
    buf[ mi16Len++ ] = 0;   // UNUSED
    buf[ mi16Len++ ] = 0;   // UNUSED

    buf[ mi16Len++ ] = 0;   // UNUSED
    buf[ mi16Len++ ] = 0;   // UNUSED

    buf[ mi16Len++ ] = 0;   // UNUSED
    buf[ mi16Len++ ] = 0;   // UNUSED

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;
    return mi16Len;
}



