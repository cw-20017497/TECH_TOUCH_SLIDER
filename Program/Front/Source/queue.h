/**
 * File : queue.h
 * 
 * Queue
**/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "prj_type.h"


typedef struct _queue_
{
    void **mUdata;
    U16  mUqueueNum;        
    U8   mUdataSize;        

    U16  mUrear;
    U16  mUfront;
} SQueue_T;

void InitQueue(SQueue_T *xStQueue, void *mUdata, U8 mUdataSize, U8 mUqueueNum );

U8 IsQueueEmpty(SQueue_T *xStQueue);
U8 IsQueueFull(SQueue_T *xStQueue);

void EnQueue(SQueue_T *xStQueue, void *xUdata);
void EnQueueFront(SQueue_T *xStQueue, void *xUdata);
U8 DeQueue(SQueue_T *xStQueue, void *xUptrData);
U8 PeekQueue(SQueue_T *xStQueue, void *xUptrData);

#endif /* __QUEUE_H__ */
