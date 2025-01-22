#ifndef __HAL_LED_H__
#define __HAL_LED_H__

// PIN2PIN leds count = 35
typedef enum
{
   // MATRIX LED
    SEG_1_A,
    SEG_1_G,    // digit 1
    SEG_1_F,
    SEG_1_E,
    SEG_1_D,
    SEG_1_B,
    SEG_1_C,

    SEG_2_A,
    SEG_2_G,    // digit 1
    SEG_2_F,
    SEG_2_E,
    SEG_2_D,
    SEG_2_B,
    SEG_2_C,

    SEG_3_A,
    SEG_3_G,    // digit 1
    SEG_3_F,
    SEG_3_E,
    SEG_3_D,
    SEG_3_B,
    SEG_3_C,

    SEG_22,
    SEG_23,
    SEG_24,
    SEG_25,
    SEG_26,
    SEG_27,
    SEG_28,
    SEG_29,
    SEG_30,
    SEG_31,
    SEG_32,
    SEG_33,
    SEG_34,
    SEG_35,

    LED_ALL,

    MAX_LED_NUM
} LedId_T;
#define MAX_LED    6       /* 5bytes * 8bits = 40 */

void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);

void HAL_SetOnOffLED(U8 *pLeds, U8 mu8Size );
void HAL_SetOnOffDuty(U8 mu8Duty);

void HAL_SetDimmingLED(U8 *pLeds, U8 mu8Size);
void HAL_SetDimmingDuty(U8 mu8Duty);

#endif /* __HAL_LED_H__ */
