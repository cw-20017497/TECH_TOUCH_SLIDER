#ifndef __PARSER_MAIN_H__
#define __PARSER_MAIN_H__

#include "prj_type.h"
#include "comm.h"

#define PKT_ACK             0x80

#define PKT_REQ_LED         0x01
#define PKT_ACK_LED         (0x80|PKT_REQ_LED)

#define PKT_REQ_KEY         0x10
#define PKT_ACK_KEY         (0x80|PKT_REQ_KEY)

I16 ReadPacket_Main( U8 id , U8 *recv_pkt );
I16 IsValidPkt_Main( U8 *buf, I16 len );
I16 ParserPkt_Main( U8 *buf, I16 len);
I16 Crc16_Main( U8 *buf, I16 len );
I16 MakePkt_Main( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_MAIN_H__ */
