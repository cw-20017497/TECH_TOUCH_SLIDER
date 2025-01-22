#ifndef __PARSER_FRONT_H__
#define __PARSER_FRONT_H__

#include "prj_type.h"
#include "comm.h"


#define PKT_FRONT_REQ_LED       0x01
#define PKT_FRONT_ACK_LED       0x81

I16 ReadPacket_Front( U8 id , U8 *recv_pkt );
I16 IsValidPkt_Front( U8 *buf, I16 len );
I16 ParserPkt_Front( U8 *buf, I16 len);
I16 Crc16_Front( U8 *buf, I16 len );
I16 MakePkt_Front( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_FRONT_H__ */
