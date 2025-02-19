#pragma interrupt INTTM01 r_tau0_channel1_interrupt

#include "hw.h"
#include "hal_led.h"
#include "hal_led_onoff.h"


#define MAX_TICK    10UL        // @100us... * 10 = 1ms  ( 100hz )
#define SYNC_DELAY    40UL        // 0 ~ 100 - Led turn on/off speed
U16 u16Cycle            = MAX_TICK;
U16 u16ConfSyncDelay    = SYNC_DELAY;
U16 u16SyncDelay        = SYNC_DELAY;

typedef struct _leds_
{
    U8 Leds[ MAX_LED ];
    U8 Duty;

    U16 ConfTick;
    U16 Tick;
} Led_T;

Led_T   OnOff;
Led_T   Dimming;


typedef void (*Action_T)(U8 mu8OnOff);
typedef struct _onoff_led_
{
    U8        Led;
    U16       TargetTick;   
    U16       CurrentTick;   
    Action_T  pfOnOff;       // LED ON
} OnOff_T;



// GROUP A
OnOff_T OnOffList[] = 
{ 
    { 0,   0, 0,   HAL_OnOffLed_11 },
    { 1,   0, 0,   HAL_OnOffLed_12 },
    { 2,   0, 0,   HAL_OnOffLed_13 },
    { 3,   0, 0,   HAL_OnOffLed_14 },
    { 4,   0, 0,   HAL_OnOffLed_15 },
    { 5,   0, 0,   HAL_OnOffLed_16 },
    { 6,   0, 0,   HAL_OnOffLed_17 },

    { 7,   0, 0,   HAL_OnOffLed_21 },
    { 8,   0, 0,   HAL_OnOffLed_22 },
    { 9,   0, 0,   HAL_OnOffLed_23 },
    { 10,  0, 0,    HAL_OnOffLed_24 },
    { 11,  0, 0,    HAL_OnOffLed_25 },
    { 12,  0, 0,    HAL_OnOffLed_26 },
    { 13,  0, 0,    HAL_OnOffLed_27 },

    { 14,  0, 0,    HAL_OnOffLed_31 },
    { 15,  0, 0,   HAL_OnOffLed_32 },
    { 16,  0, 0,   HAL_OnOffLed_33 },
    { 17,  0, 0,   HAL_OnOffLed_34 },
    { 18,  0, 0,   HAL_OnOffLed_35 },
    { 19,  0, 0,   HAL_OnOffLed_36 },
    { 20,  0, 0,   HAL_OnOffLed_37 },

    { 21,  0, 0,   HAL_OnOffLed_41 },
    { 22,  0, 0,   HAL_OnOffLed_42 },
    { 23,  0, 0,   HAL_OnOffLed_43 },
    { 24,  0, 0,   HAL_OnOffLed_44 },
    { 25,  0, 0,   HAL_OnOffLed_45 },
    { 26,  0, 0,   HAL_OnOffLed_46 },
    { 27,  0, 0,   HAL_OnOffLed_47 },

    { 28,  0, 0,   HAL_OnOffLed_51 },
    { 29,  0, 0,   HAL_OnOffLed_52 },
    { 30,  0, 0,   HAL_OnOffLed_53 },
    { 31,  0, 0,   HAL_OnOffLed_54 },
    { 32,  0, 0,   HAL_OnOffLed_55 },
    { 33,  0, 0,   HAL_OnOffLed_56 },
    { 34,  0, 0,   HAL_OnOffLed_57 },

    { 36,  0, 0,   HAL_OnOffLed_61 },
    { 37,  0, 0,   HAL_OnOffLed_62 },
    { 38,  0, 0,   HAL_OnOffLed_63 },
    { 39,  0, 0,   HAL_OnOffLed_64 },
    { 40,  0, 0,   HAL_OnOffLed_65 },
    { 41,  0, 0,   HAL_OnOffLed_66 },
    { 42,  0, 0,   HAL_OnOffLed_67 },

    { 43,  0, 0,   HAL_OnOffLed_71 },
    { 44,  0, 0,   HAL_OnOffLed_72 },
    { 45,  0, 0,   HAL_OnOffLed_73 },
    { 46,  0, 0,   HAL_OnOffLed_74 },
    { 47,  0, 0,   HAL_OnOffLed_75 }
};          
#define SZ_ONOFF_LIST       (sizeof(OnOffList)/sizeof(OnOff_T))

// Check led bit
// return : true or false
static U8 IsSetBit(U8 *pBuf, U8 mu8Val)
{
    U8 mu8Byte;
    U8 mu8Bit;
    U8 mu8OnOff;

    mu8Byte = mu8Val / 8;
    mu8Bit = mu8Val % 8;
    mu8OnOff = (U8)(pBuf[ mu8Byte ] & (1U<<mu8Bit));
    if( mu8OnOff != 0 )
    {
        return ON;    // Set bit
    }

    return OFF;   // Cleared bit
}


static U16 CalcDuty2Tick( U8 mu8Duty )
{
    U16 mu16Tick = 0;

    if( mu8Duty > 100 )
    {
        mu8Duty = 100;
    }
    else if( mu8Duty < 1 )
    {
        mu8Duty = 1;
    }

    mu16Tick = (U16)((MAX_TICK * (U16)mu8Duty) / 100UL);

    return mu16Tick;
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

void HAL_SetOnOffLED(U8 *pLeds, U8 mu8Size, U8 Duty )
{
    U8 i;
    U8 mu8Byte;
    U8 mu8Bit;

    for( i = 0; i < LED_ALL; i++ )
    {
        mu8Byte = i / 8;
        mu8Bit = i % 8;
        if( (*(pLeds + mu8Byte) & (1<<mu8Bit)) != 0 )
        {
            OnOffList[ i ].TargetTick = CalcDuty2Tick( Duty );
        }
        else
        {
            OnOffList[ i ].TargetTick = 0;
        }
    }
}


void HAL_SetOnOffDuty(U8 mu8Duty)
{
    OnOff.Duty = mu8Duty;
    OnOff.ConfTick =  CalcDuty2Tick( mu8Duty );
}


void HAL_SetDimmingLED(U8 *pLeds, U8 mu8Size)
{
    memcpy( &Dimming.Leds[0], pLeds, mu8Size );
}


void HAL_SetDimmingDuty(U8 mu8Duty)
{
    Dimming.Duty = mu8Duty;
    Dimming.ConfTick = CalcDuty2Tick( mu8Duty );
}




// mu32Led : LED ON/OFF
// mu32Dimming : LED DIMMING ON/OFF
// muOnOff : Duty ON/OFF
static void OnOffLed(OnOff_T *pList, U8 mu8ListSize, U8 *pOnOff, U8 *pDimming, U8 mu8OnOff)
{
    U8 i;
    U8 mu8Led;
    Action_T    pfOnOff;      // LED ON/OFF

    for( i = 0 ; i < mu8ListSize; i++ )
    {
        mu8Led = pList[ i ].Led;

        if( IsSetBit(pOnOff, mu8Led ) == ON )
        {
            // LED가 ON이면, ON/OFF 명령에 따라 제어
            pfOnOff = pList[ i ].pfOnOff;
            if( pfOnOff != NULL )
            {
                pfOnOff( mu8OnOff );
            }
        }
    }
}

// DIMMING 제어인 경우,
static void OnOffDimming(OnOff_T *pList, U8 mu8ListSize, U8 *pOnOff, U8 *pDimming, U8 mu8OnOff)
{
    U8 i;
    U8 mu8Led;
    Action_T    pfOnOff;      // LED ON/OFF

    for( i = 0 ; i < mu8ListSize; i++ )
    {
        mu8Led = pList[ i ].Led;

        // LED가 OFF 일때, Dimming 제어
        if( IsSetBit(pOnOff, mu8Led ) == OFF )
        {
            pfOnOff = pList[ i ].pfOnOff;

            if( pfOnOff != NULL )
            {
                // Dimming 제어
                if( IsSetBit(pDimming, mu8Led ) == ON )
                {
                    // DUTY ON/OFF
                    pfOnOff( mu8OnOff );
                    if( mu8OnOff == ON )
                    {
                        pfOnOff( ON );
                    }
                    else
                    {
                        pfOnOff( OFF );
                    }
                }
                else
                {
                    pfOnOff( OFF );
                }
            }
        }
    }
}



/*
 * 일반 버튼 LED와 추출 LED의 Tick을 구분한다.
 * 이유는 LED 사출 형태가 서로 달라서 밝기 조정을 별도로 조정해야한다.
 *
 */

#if 0 
static void ControlLed(void)
{
    if( u16Cycle == 0 )
    {
        u16Cycle        = MAX_TICK;

        // Group A
        Dimming.Tick    = Dimming.ConfTick; 
        OnOff.Tick      = OnOff.ConfTick;
    }

    if( u16Cycle != 0 )
    {
        --u16Cycle;
    }

    // DIMMING DUTY 제어
    if( Dimming.Tick != 0 )
    {
        // ON 
        Dimming.Tick--;
        OnOffDimming( OnOffList, SZ_ONOFF_LIST, &OnOff.Leds[0], &Dimming.Leds[0], ON );
    }
    else
    {
        // OFF 
        OnOffDimming( OnOffList, SZ_ONOFF_LIST, &OnOff.Leds[0], &Dimming.Leds[0], OFF );
    }

    // LED DUTY 제어
    if( OnOff.Tick != 0 )
    {
        // ON 
        OnOff.Tick--;
        OnOffLed( OnOffList, SZ_ONOFF_LIST, &OnOff.Leds[0], &Dimming.Leds[0], ON );
    }
    else
    {
        // OFF 
        OnOffLed( OnOffList, SZ_ONOFF_LIST, &OnOff.Leds[0], &Dimming.Leds[0], OFF );
    }
}
#endif
static void OnOffTick( OnOff_T *pLed )
{
    if( u16Cycle < pLed->CurrentTick )
    {
        pLed->pfOnOff( ON );
    }
    else
    {
        pLed->pfOnOff( OFF );
    }

}


// Operation to smoothly control the led
static void Sync( OnOff_T *pLed )
{
    if( pLed->TargetTick > pLed->CurrentTick )
    {
        pLed->CurrentTick++;
    }
    else if( pLed->TargetTick < pLed->CurrentTick )
    {
        pLed->CurrentTick--;
    }

}

static void ControlLed(void)
{
    U8 i;

    u16Cycle++;
    if( u16Cycle > MAX_TICK )
    {
        u16Cycle = 0;
    }

    u16SyncDelay--;
    for( i = 0; i < SZ_ONOFF_LIST ; i++ )
    {
        OnOffTick( &OnOffList[ i ] );
        if( u16SyncDelay == 0 )
        {
            Sync( &OnOffList[ i ] );
        }
    }

    if( u16SyncDelay == 0 )
    {
        u16SyncDelay = u16ConfSyncDelay;
    }
}


// 디밍 제어 1ms
// 처리 시간 : 0.260 ms
__interrupt static void r_tau0_channel1_interrupt(void)
{
    EI();
    ControlLed();
}

