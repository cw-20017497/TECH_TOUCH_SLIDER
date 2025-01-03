#ifndef __PARSER_KEY_H__
#define __PARSER_KEY_H__

#include "prj_type.h"
#include "comm.h"


I16 ReadPacket_Key( U8 id , U8 *recv_pkt );
I16 IsValidPkt_Key( U8 *buf, I16 len );
I16 ParserPkt_Key( U8 *buf, I16 len);
I16 Crc16_Key( U8 *buf, I16 len );
I16 MakePkt_Key( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_KEY_H_ */
