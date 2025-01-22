#ifndef __COMM_H__
#define __COMM_H__

#include "prj_type.h"
#include "hal_serial.h"

#define COMM_ID_KEY          UART_ID_0
#define COMM_ID_MAIN         UART_ID_1

#define MAX_COMM_ID     MAX_UART_ID

typedef U16 CommHeader_T;

CommHeader_T GetCommHeader( U8 id );
void    SetCommHeader( U8 id, CommHeader_T val );

I16     CommSendPacket( U8 id,  U8 *send_pkt, I16  len );
//I16  CommRecvPacket( U8 id , U8 *recv_pkt );

#endif /* __COMM_H__ */
