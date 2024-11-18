#include "hw.h"
#include "hal_led.h"


#define ALL_ON      0xFFU
#define ALL_OFF     0x00U


typedef struct _leds_
{
    U8 Leds[ MAX_LED_BUF ];
    U8 Duty;
} Led_T;

Led_T   OnOff;
Led_T   Dimming;

Led_T   LastedOnOff;
Led_T   LastedDimming;


void HAL_InitLed(void)
{
    MEMSET((void __FAR *)&OnOff.Leds[0], OFF, MAX_LED_BUF );
    MEMSET((void __FAR *)&Dimming.Leds[0], OFF, MAX_LED_BUF );
    OnOff.Duty = DEFAULT_ONOFF_DUTY;
    Dimming.Duty = DEFAULT_DIMMING_DUTY;

    HAL_UpdateLed();
}

// ON/OFF
void HAL_TurnOnOffLEDAll(U8 mu8OnOff)
{
    if( mu8OnOff == ON )
    {
        MEMSET((void __FAR *)&OnOff.Leds[0], ALL_ON, MAX_LED_BUF );
    }
    else 
    {
        MEMSET((void __FAR *)&OnOff.Leds[0], ALL_OFF, MAX_LED_BUF );
    }
}


void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;

    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        OnOff.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        OnOff.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }
}

void HAL_TurnOnOffLEDEx(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;

    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        OnOff.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        OnOff.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }

}

void HAL_GetLedOnOffStatus(U8 *pBuf)
{
    MEMCPY( (void __FAR *)pBuf, (const void __FAR *)&OnOff.Leds[0], MAX_LED_BUF );
}

void HAL_SetLedDuty(U8 mu8Duty)
{
    OnOff.Duty = mu8Duty;
    if( OnOff.Duty > 100 )
    {
        OnOff.Duty = 100;
    }
}

U8 HAL_GetLedDuty(void)
{
    return OnOff.Duty;
}


// DIMMING 
void HAL_DimmingLedAll(U8 mu8OnOff)
{
    if( mu8OnOff == ON )
    {
        MEMSET((void __FAR *)&Dimming.Leds[0], ALL_ON, MAX_LED_BUF );
    }
    else 
    {
        MEMSET((void __FAR *)&Dimming.Leds[0], ALL_OFF, MAX_LED_BUF );
    }
}

void HAL_DimmingLed(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;


    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        Dimming.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        Dimming.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }
}

void HAL_GetLedDimmingStatus(U8 *pBuf)
{
    MEMCPY( (void __FAR *)pBuf, (const void __FAR *)&Dimming.Leds[0], MAX_LED_BUF );
}

void HAL_SetLedDimmingDuty(U8 mu8Duty)
{
    Dimming.Duty = mu8Duty;
    if( mu8Duty > 100 )
    {
        Dimming.Duty = 100;
    }
}

U8 HAL_GetLedDimmingDuty(void)
{
    return Dimming.Duty;
}


U8 HAL_IsUpdateLed(void)
{
    U16 i;
    U8 *pSrc = NULL;
    U8 *pDest = NULL;
    U8 *pSrc_1 = NULL;
    U8 *pDest_1 = NULL;

    pSrc = (U8 *)&OnOff;
    pDest = (U8 *)&LastedOnOff;

    pSrc_1 = (U8 *)&Dimming;
    pDest_1 = (U8 *)&LastedDimming;
    for( i = 0; i < sizeof(Led_T) ; i++ )
    {
        if( *(pSrc+i) != *(pDest + i)
                || *(pSrc_1+i) != *(pDest_1 + i ) )
        {
            return TRUE; // update
        }
    }

    return FALSE;
}

void HAL_UpdateLed(void)
{
    MEMCPY( (void __FAR *)&LastedDimming, (const void __FAR *)&Dimming, sizeof( Led_T) );
    MEMCPY( (void __FAR *)&LastedOnOff, (const void __FAR *)&OnOff, sizeof( Led_T ));
}
