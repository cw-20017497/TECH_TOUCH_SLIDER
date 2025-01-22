#pragma interrupt INTTM01 r_tau0_channel1_interrupt

#include "hw.h"
#include "hal_led.h"
#include "hal_led_onoff.h"


#define MAX_TICK    10UL        // @100us... * 10 = 1ms  ( 100hz )
U16 u16Cycle            = MAX_TICK;
U16 GroupB_DimmingTick  = MAX_TICK;
U16 GroupB_OnOffTick    = MAX_TICK;

typedef struct _leds_
{
    U8 Leds[ MAX_LED ];
    U8 Duty;

    U16 ConfTick;
    U16 Tick;
} Led_T;

Led_T   OnOff;
Led_T   Dimming;


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

void HAL_SetOnOffLED(U8 *pLeds, U8 mu8Size )
{
    memcpy( &OnOff.Leds[0], pLeds, mu8Size );
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



typedef void (*Action_T)(U8 mu8OnOff);
typedef struct _onoff_led_
{
    U8        Led;
    Action_T  pfOnOff;       // LED ON
} OnOff_T;



// GROUP A
static OnOff_T Group_A_List[] = 
{ 
    { 0,      HAL_OnOffLed_11 },
    { 1,      HAL_OnOffLed_12 },
    { 2,      HAL_OnOffLed_13 },
    { 3,      HAL_OnOffLed_14 },
    { 4,      HAL_OnOffLed_15 },
    { 5,      HAL_OnOffLed_16 },
    { 6,      HAL_OnOffLed_17 },

    { 7,      HAL_OnOffLed_21 },
    { 8,      HAL_OnOffLed_22 },
    { 9,      HAL_OnOffLed_23 },
    { 10,      HAL_OnOffLed_24 },
    { 11,      HAL_OnOffLed_25 },
    { 12,      HAL_OnOffLed_26 },
    { 13,      HAL_OnOffLed_27 },

    { 14,      HAL_OnOffLed_31 },
    { 15,     HAL_OnOffLed_32 },
    { 16,     HAL_OnOffLed_33 },
    { 17,     HAL_OnOffLed_34 },
    { 18,     HAL_OnOffLed_35 },
    { 19,     HAL_OnOffLed_36 },
    { 20,     HAL_OnOffLed_37 },

    { 21,     HAL_OnOffLed_41 },
    { 22,     HAL_OnOffLed_42 },
    { 23,     HAL_OnOffLed_43 },
    { 24,     HAL_OnOffLed_44 },
    { 25,     HAL_OnOffLed_45 },
    { 26,     HAL_OnOffLed_46 },
    { 27,     HAL_OnOffLed_47 },

    { 28,     HAL_OnOffLed_51 },
    { 29,     HAL_OnOffLed_52 },
    { 30,     HAL_OnOffLed_53 },
    { 31,     HAL_OnOffLed_54 },
    { 32,     HAL_OnOffLed_55 },
    { 33,     HAL_OnOffLed_56 },
    { 34,     HAL_OnOffLed_57 },

    { 36,     HAL_OnOffLed_61 },
    { 37,     HAL_OnOffLed_62 },
    { 38,     HAL_OnOffLed_63 },
    { 39,     HAL_OnOffLed_64 },
    { 40,     HAL_OnOffLed_65 },
    { 41,     HAL_OnOffLed_66 },
    { 42,     HAL_OnOffLed_67 },

    { 43,     HAL_OnOffLed_71 },
    { 44,     HAL_OnOffLed_72 },
    { 45,     HAL_OnOffLed_73 },
    { 46,     HAL_OnOffLed_74 },
    { 47,     HAL_OnOffLed_75 }
};          
#define SZ_GROUP_A_LIST       (sizeof(Group_A_List)/sizeof(OnOff_T))

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


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // GROUP A 
    //

    // DIMMING DUTY 제어
    if( Dimming.Tick != 0 )
    {
        // ON 
        Dimming.Tick--;
        OnOffDimming( Group_A_List, SZ_GROUP_A_LIST, &OnOff.Leds[0], &Dimming.Leds[0], ON );
    }
    else
    {
        // OFF 
        OnOffDimming( Group_A_List, SZ_GROUP_A_LIST, &OnOff.Leds[0], &Dimming.Leds[0], OFF );
    }

    // LED DUTY 제어
    if( OnOff.Tick != 0 )
    {
        // ON 
        OnOff.Tick--;
        OnOffLed( Group_A_List, SZ_GROUP_A_LIST, &OnOff.Leds[0], &Dimming.Leds[0], ON );
    }
    else
    {
        // OFF 
        OnOffLed( Group_A_List, SZ_GROUP_A_LIST, &OnOff.Leds[0], &Dimming.Leds[0], OFF );
    }
}


// 디밍 제어 1ms
// 처리 시간 : 0.260 ms
__interrupt static void r_tau0_channel1_interrupt(void)
{
    EI();
    ControlLed();
}

