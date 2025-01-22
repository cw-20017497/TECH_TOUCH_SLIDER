#ifndef __DISPLAY_LIB_H__
#define __DISPLAY_LIB_H__

#include "prj_type.h"



void InitDispTimer(void);

// TIMER COUNTER 
#define TIMER_100MS    10UL
#define TIMER_500MS    50UL
#define TIMER_1SEC     100UL
#define TIMER_2SEC     200UL
#define TIMER_3SEC     300UL
#define TIMER_4SEC     400UL
#define TIMER_5SEC     500UL
#define TIMER_6SEC     600UL
#define TIMER_7SEC     700UL
#define TIMER_20SEC    2000UL
#define TIMER_30SEC    3000UL
#define TIMER_20MIN    120000UL
#define TIMER_60MIN    360000UL

// TIMER INDEX
enum
{
    DISP_TIMER_CONFIG,       /* 설정 */

    MAX_DISP_KEY_TIMER_ID       
};

U8 IsExpiredDispTimer(U8 id);
void SetDispTimer(U8 id, U16 time);


// TIMER BLINK INDEX 
#define DISP_TIMER_ID_500MS        0    /* LED 점멸 타이머 0.5sec */
#define DISP_TIMER_ID_1SEC         1    /* LED 점멸 타이머 1sec */
#define MAX_DISP_TIMER_BLINK_ID    2
U8 IsExpiredDispBlinkTimer(U8 id);
U8 BlinkLED( U8 mu8OnOff, void (*pFun)(U8 mu8OnOff), U8 mu8TimerId );

void UpdateDisplayTimer(void);

#endif /* __DISPLAY_LIB_H__ */
