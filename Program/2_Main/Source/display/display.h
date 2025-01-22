#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "prj_type.h"


// 7세그먼트 숫자 표시( 0 ~ 999 )
void DispSegmentDigit(U16 mu16Val);

// 7세그먼트 문자 표시 ID
enum
{
    SEGMENT_CHAR_ID_ON,           /* On */
    SEGMENT_CHAR_ID_OFF,          /* OFF */
    SEGMENT_CHAR_ID_CON,          /* Con */
    SEGMENT_CHAR_ID_STACK_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_STACK_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_BAR_BOTTOM,   /* ___ */
    SEGMENT_CHAR_ID_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_CHP,          /* CHP */
    SEGMENT_CHAR_ID_CP,           /* CP  */
    SEGMENT_CHAR_ID_CIRCLE,       /* CIRCLE  */
    SEGMENT_CHAR_ID_SPACE,        /* SPACE  */
    SEGMENT_CHAR_ID_ICE_1,         /* ICE OUT-EFFECT 1  */
    SEGMENT_CHAR_ID_ICE_2,         /* ICE OUT-EFFECT 2 */
    SEGMENT_CHAR_ID_ICE_3,         /* ICE OUT-EFFECT 3 */
    SEGMENT_CHAR_ID_ICE_4,         /* ICE OUT-EFFECT 4 */
    SEGMENT_CHAR_ID_CENTER_1,      /* SOUND VOL -1- */
    SEGMENT_CHAR_ID_CENTER_2,      /* SOUND VOL -2- */
    SEGMENT_CHAR_ID_CENTER_3,      /* SOUND VOL -3- */
    SEGMENT_CHAR_ID_SE,             /* CUSTOMER MODE - SELF */
    SEGMENT_CHAR_ID_CO,             /* CUSTOMER MODE - COWAY(?) - VISIT */
    SEGMENT_CHAR_ID_STE,            /* 살균 - StE */
    MAX_SEGMENT_CHAR_ID,        
};

void DispSegmentChar(U8 mu8CharId);

void DispSegmentOff(void);

// 연속 추출 화면 표시
void DispSegmentCon(U8 mu8Step);

/* Functinos */
void TurnOffAllLED(void);
void TurnOnAllLED(void);

void DispLedDuty(void);

void ResetInfinityCount(void);
void DispWaterOutAmountInfinity(void);

// 온도 표시
void DispTemp(U16 mu16Val);

void DispBar(U8 pos);
void DispBarStack(U8 pos);


#endif /* __DISPLAY_H__ */
