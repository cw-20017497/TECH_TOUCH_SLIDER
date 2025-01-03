#ifndef __COMM_QUEUE_H__
#define __COMM_QUEUE_H__

#include "prj_type.h"
#include "hal_serial.h"
#include "parser_front.h"


typedef struct _comm_data_
{
    U16 packet;    
    U8  retry_count;
} CommData_T;

void InitCommQueue(void);


// FRONT 
void SetCommQueueFront(U8 packet);

void SendPacketQueueFront( void );

void ReceivedFrontAck(U8 packet);

#endif /* __COMM_H__ */
