#ifndef __HAL_LED_H__
#define __HAL_LED_H__


#define MAX_LED_BUF        6               /* 12bytes x 8bits = 96bits */

#define DEFAULT_ONOFF_DUTY          90U
#define DEFAULT_DIMMING_DUTY        30U
#define DEFAULT_ONOFF_DUTY_DARK     50U
#define DEFAULT_DIMMING_DUTY_DARK   20U
typedef enum
{
   // MATRIX LED
    SEG_1_A,        // digit 100
    SEG_1_F,    
    SEG_1_G,
    SEG_1_E,
    SEG_1_D,
    SEG_1_C,
    SEG_1_B,

    SEG_2_A,        // digit 10
    SEG_2_F,    
    SEG_2_G,
    SEG_2_E,
    SEG_2_D,
    SEG_2_C,
    SEG_2_B,

    SEG_3_A,        // digit 1
    SEG_3_F,    
    SEG_3_G,
    SEG_3_E,
    SEG_3_D,
    SEG_3_C,
    SEG_3_B,


    SEG_BAR_14,     // SEG_22,
    SEG_BAR_13,     // SEG_23,
    SEG_BAR_12,     // SEG_24,
    SEG_BAR_11,     // SEG_25,
    SEG_BAR_10,     // SEG_26,
    SEG_BAR_9,     // SEG_27,
    SEG_BAR_8,     // SEG_28,
    SEG_BAR_7,     // SEG_29,
    SEG_BAR_6,     // SEG_30,
    SEG_BAR_5,     // SEG_31,
    SEG_BAR_4,     // SEG_32,
    SEG_BAR_3,     // SEG_33,
    SEG_BAR_2,     // SEG_34,
    SEG_BAR_1,     // SEG_35,

    LED_ALL,

    MAX_LED_NUM
} LedId_T;


void HAL_InitLed(void);

// LED ON/OFF 
void HAL_TurnOnOffLEDAll(U8 mu8OnOff);
void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);
void HAL_TurnOnOffLEDEx(LedId_T led, U8 mu8OnOff);
void HAL_GetLedOnOffStatus(U8 *pBuf);


// LED DUTY 
void HAL_SetLedDuty(U8 mu8Duty);
U8 HAL_GetLedDuty(void);

// DIMMING ON/OFF
void HAL_DimmingLedAll(U8 mu8OnOff);
void HAL_DimmingLed(LedId_T led, U8 mu8OnOff);
void HAL_GetLedDimmingStatus(U8 *pBuf);

// DIMMING DUTY 
void HAL_SetLedDimmingDuty(U8 mu8Duty);
U8 HAL_GetLedDimmingDuty(void);


U8 HAL_IsUpdateLed(void);
void HAL_UpdateLed(void);

#endif /* __HAL_LED_H__ */
