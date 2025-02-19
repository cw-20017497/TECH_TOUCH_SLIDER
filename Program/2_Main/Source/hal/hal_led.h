#ifndef __HAL_LED_H__
#define __HAL_LED_H__


#define MAX_LED_BUF        7               /* 12bytes x 8bits = 96bits */

#define DEFAULT_ONOFF_DUTY          90U
#define DEFAULT_DIMMING_DUTY        30U
#define DEFAULT_ONOFF_DUTY_DARK     50U
#define DEFAULT_DIMMING_DUTY_DARK   20U
typedef enum
{
   // MATRIX LED
    SEG_BAR_7,     // SEG_28,       //1
    SEG_BAR_8,     // SEG_28,       //1
    SEG_BAR_9,     // SEG_27,
    SEG_BAR_10,     // SEG_26,
    SEG_BAR_11,     // SEG_25,
    SEG_BAR_12,     // SEG_24,
    SEG_BAR_13,     // SEG_23,

    SEG_1_F,        // digit 100        // 8
    SEG_1_A,    
    SEG_1_B,
    SEG_1_G,
    SEG_1_C,
    SEG_1_D,
    SEG_1_E,        

    SEG_2_F,        // digit 10
    SEG_2_A,    
    SEG_2_B,
    SEG_2_G,
    SEG_2_C,
    SEG_2_D,
    SEG_2_E,

    SEG_3_F,        // digit 1
    SEG_3_A,    
    SEG_3_B,
    SEG_3_G,
    SEG_3_C,
    SEG_3_D,
    SEG_3_E,


    SEG_CIRCLE_1,     // SEG_29,
    SEG_CIRCLE_2,     // SEG_30,
    SEG_CIRCLE_3,     // SEG_31,
    SEG_CIRCLE_4,     // SEG_32,
    SEG_CIRCLE_5,     // SEG_33,
    SEG_CIRCLE_6,     // SEG_34,
    SEG_CIRCLE_7,     // SEG_35,
    SEG_CIRCLE_UNUSED,     // SEG_35,


    SEG_BAR_1, // PBA VER2
    SEG_BAR_2, 
    SEG_BAR_3, 
    SEG_BAR_4, 
    SEG_BAR_5, 
    SEG_BAR_6, 
    //SEG_BAR_7, 

    SEG_CIRCLE_8, 
    SEG_CIRCLE_9, 
    SEG_CIRCLE_10,
    SEG_CIRCLE_11,
    SEG_CIRCLE_12,
    
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
