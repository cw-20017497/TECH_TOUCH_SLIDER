#include "comm.h"

/***********************************************************************************************
 *  HEADER
 */

CommHeader_T    comm_header[ MAX_COMM_ID ];


CommHeader_T GetCommHeader( U8 id )
{
    // Get address of data model 
    return comm_header[ id ];
}

void    SetCommHeader( U8 id, CommHeader_T val )
{
    // Set address of data model 
    comm_header[ id ] = val;
}



/***********************************************************************************************
 *  SEND PACKET 
 */
I16 CommSendPacket( U8 id,  U8 *send_pkt , I16  len )
{
    U8 i;


    if( HAL_IsFullSendBuffer( id ) != TRUE )
    {
        for( i = 0 ; i < len ; i++ )
        {
            HAL_SetSendBuffer( id, send_pkt[ i ] );
        }

        HAL_SendByte( id );
        return len; /* SENT BUF SIZE */
    }

    return -1;  /* ERROR - THERE IS FULL DATA */
}


/***********************************************************************************************
 *  RECV PACKET 
 */
#if 0
I16 CommRecvPacket( U8 id , U8 *recv_pkt )
{
#if 0
    U16  i;
    I16 len;

    if( HAL_IsEmptyRecvBuffer( id ) != TRUE )
    {
        for( i = 0; i < len ; i++ )
        {
            recv_pkt[ i ] = HAL_GetRecvBuffer( id, i );
        }

        return len; /* RECEIVED BUF SIZE */
    }

    return -1;  /* ERROR - THERE IS NO DATA */
#else
    U16  i = 0;
    I16 len = 0;

    while( HAL_IsEmptyRecvBuffer( id ) == FALSE )
    {
        recv_pkt[ i++ ] = HAL_GetRecvBuffer( id );
        len++;
    }

    return len; /* RECEIVED BUF SIZE */
#endif
}

I16 CommRecvPacketByOne( U8 id , U8 *recv_pkt, U8 stx, U8 etx )
{
    U16  i = 0;
    I16 len = 0;
    U8 buf;
    U8 startRead = FALSE;

    while( HAL_IsEmptyRecvBuffer( id ) == FALSE )
    {
        buf = HAL_GetRecvBuffer( id );
        if( buf == stx )
        {
            startRead = TRUE;
            recv_pkt[ len++ ] = buf;
        }

        if( startRead == TRUE )
        {
            recv_pkt[ len++ ] = buf;
            if( buf == etx )
            {
                break;
            }
        }
    }

    return len; /* RECEIVED BUF SIZE */
}
#endif

