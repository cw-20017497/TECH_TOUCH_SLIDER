#ifndef __COMM_QUEUE_H__
#define __COMM_QUEUE_H__

#include "prj_type.h"
#include "hal_serial.h"


typedef struct _comm_data_
{
    U16 packet;    
    U8  retry_count;
} CommData_T;

void InitCommQueue(void);


// Main 
void SetCommQueueMain(U8 packet);

void SendPacketQueueMain( void );

void ReceivedMainAck(U8 packet);

#endif /* __COMM_H__ */
