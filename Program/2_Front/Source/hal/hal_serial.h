#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "prj_type.h"

/* UART ID */
#define UART_ID_0         0
#define UART_ID_1         1
#define UART_ID_2         2
#define UART_ID_3         3
#define MAX_UART_ID       4


void    HAL_InitCommId( U8 uart_id );
void    HAL_InitComm( void );

U8  HAL_IsFullRecvBuffer( U8 uart_id );
U8  HAL_IsEmptyRecvBuffer( U8 uart_id );
U8  HAL_IsFullSendBuffer( U8 uart_id );
U8  HAL_IsEmptySendBuffer( U8 uart_id );

void HAL_InitRecvLength(U8 uart_id );
I16  HAL_GetRecvLength( U8 uart_id );
I16  HAL_GetSendLength( U8 uart_id );

void    HAL_SetRecvBuffer(U8 uart_id, U8 _byte );
//U8    HAL_GetRecvBuffer( U8 uart_id, U16 index );
U8 HAL_GetRecvBuffer( U8 uart_id );

void    HAL_SetSendBuffer( U8 uart_id, U8 _byte );
U8     HAL_GetSendBuffer( U8 uart_id, U16 index );

void    HAL_SendByte( U8 uart_id );

#endif /* __HAL_SERIAL_H__ */

