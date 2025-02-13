#include "hw.h"

#include "display_slide_temp.h"

#include "slider.h"
#include "display.h"
#include "sound.h"



#define SLIDER_BAR_LEVEL    16
#define DEFAULT_TEMP        100
#define TICK_TEMP           1

/* MODE TIMER */
#define SLIDER_LEVEL_TIMER       30  /* 300ms@10ms*/
#define SLIDER_MODE_TIMER       500 /* 5sec@10ms*/

/* LEVEL MODE MIN/MAX */
#define MIN_TEMP        45
#define MAX_TEMP        100

#define UPDATE_DELAY    2

typedef struct _temp_data_
{
    U16 SlideVal;
    U8 Temp;
} TempData_T;

#define MAX_TEMP_DATA_NUM  4
TempData_T tempDataList[ MAX_TEMP_DATA_NUM ] = 
{
    { 0,    45 },
    { 64,   70 },
    { 128,  85 },
    { 192,  100 }
};


typedef struct _slide_temp_
{
    U8 Mode;
    U16 ModeTimer;
    U16 LevelTimer;

    U16 Temp;
    U16 SliderVal;
    U16 SliderAmount;
    U16 SliderRemainAmount;

    /* FINE TOUCH */
    U8 FineDir;
    U16 FineVal;
    U16 FineRestVal;
    U16 FineTemp;
}DispSliderTemp_T;

DispSliderTemp_T SliderTemp;


void InitSliderTemp(void)
{
    SliderTemp.Mode = SLIDER_TEMP_MODE_READY;
    SliderTemp.Temp = 100;
}

void SetSliderTempMode(U8 mode)
{
    SliderTemp.Mode = mode;
}

U8 GetSliderTempMode(void)
{
    return SliderTemp.Mode;
}

void SetSliderTempVal(U16 val)
{
    SliderTemp.SliderVal = val;
}

void SetSliderTempAmount(U16 SlideVal)
{
    SliderTemp.SliderAmount = SlideVal;
}

void SetSliderTempFineVal(U16 val, U8 dir)
{
    SliderTemp.FineVal = val;
    SliderTemp.FineDir = dir;
}

U8 IsExpiredTempLevelTimer(void)
{
    if( SliderTemp.LevelTimer == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void RefreshSliderTempLevelMode(void)
{
    SliderTemp.LevelTimer = SLIDER_LEVEL_TIMER;
}

U8 IsExpiredTempModeTimer(void)
{
    if( SliderTemp.ModeTimer == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void ClearSliderTempMode(void)
{
    SliderTemp.ModeTimer = 0;
}

void RefreshSliderTempMode(void)
{
    SliderTemp.ModeTimer = SLIDER_MODE_TIMER;
}

static U8 GetTempMinMax(U16 temp)
{
    if( temp < MIN_TEMP )
    {
        temp = MIN_TEMP;
    }
    else if( temp > MAX_TEMP )
    {
        temp = MAX_TEMP;
    }

    return temp;
}



static U8 GetTempIndexFromSlide(U16 slide)
{
    U8 retIndex = 0;

    if( slide > tempDataList[3].SlideVal )       { retIndex = 3; }
    else if( slide > tempDataList[2].SlideVal )  { retIndex = 2; }
    else if( slide > tempDataList[1].SlideVal )  { retIndex = 1; }
    else                                       { retIndex = 0; }

    return retIndex;
}

static U8 GetTempFromSlide(U16 slide)
{
    U8 index;

    index = GetTempIndexFromSlide( slide );
    return tempDataList[ index ].Temp;
}


static U16 calcTempAmount(U16 Val, U16 *pTempAmount)
{
    U16 restAmount;

    *pTempAmount = Val / SLIDER_BAR_LEVEL;
    restAmount = Val % (SLIDER_BAR_LEVEL * (*pTempAmount));

    return restAmount;
}



static void SmoothUpdateTempDisplay(U8 isInit, U16 initTemp, U16 targetTemp)
{
    static U16 currentTemp = 0;
    static U16 updateDelay = UPDATE_DELAY;


    // setting currrnet temp 
    if( isInit == TRUE )
    {
        currentTemp = initTemp;
        return ;
    }

    if (updateDelay > 0)
    {
        updateDelay--;
    }
    else if (currentTemp != targetTemp)
    {
        currentTemp += (targetTemp > currentTemp) ? TICK_TEMP : -TICK_TEMP;
        updateDelay = UPDATE_DELAY;
    }

    DispTemp(currentTemp);
}


static void PlayTempChangeBeep( U16 currentTemp )
{
    static U16 prevTemp;

    if( prevTemp != currentTemp )
    {
        prevTemp = currentTemp;
        Sound( SOUND_SELECT );
    }
}


void ProcessDisplaySliderTemp(void)
{
    U8 tempBar = 0;
    U8 direction = 0;


    // Reset Mode by timer
    if( SliderTemp.ModeTimer != 0 )
    {
        SliderTemp.ModeTimer--;
    }
    else
    {
        if( SliderTemp.Mode == SLIDER_TEMP_MODE_LEVEL )
        {
            SliderTemp.Mode  = SLIDER_TEMP_MODE_READY;
        }
        else if( SliderTemp.Mode == SLIDER_TEMP_MODE_FINE )
        {
            SliderTemp.Mode  = SLIDER_TEMP_MODE_READY;
        }
    }

    if( SliderTemp.LevelTimer != 0 )
    {
        SliderTemp.LevelTimer--;
    }
    else
    {
        if( SliderTemp.Mode == SLIDER_TEMP_MODE_LEVEL )
        {
            SliderTemp.Mode  = SLIDER_TEMP_MODE_FINE;
        }
    }

    // display mode
    switch( SliderTemp.Mode )
    {
        case SLIDER_TEMP_MODE_READY:
            if( SliderTemp.ModeTimer == 0 )
            {
                TurnOffAllLED();
                SliderTemp.Temp = DEFAULT_TEMP;
                DispTemp( SliderTemp.Temp );
            }
            break;

        case SLIDER_TEMP_MODE_LEVEL:
            // display slider bar
            tempBar = GetTempIndexFromSlide( SliderTemp.SliderVal );
            if( tempBar == 0 ) tempBar = 4;
            else if( tempBar == 1 ) tempBar = 8;
            else if( tempBar == 2 ) tempBar = 11;
            else if( tempBar == 3 ) tempBar = 14;
            DispBarStack( tempBar );


            // display temperture
            SliderTemp.Temp = GetTempFromSlide( SliderTemp.SliderVal );
            DispTemp( SliderTemp.Temp );

            // reset variable
            SliderTemp.FineRestVal = 0;
            SmoothUpdateTempDisplay( TRUE, SliderTemp.Temp, 0 );

            break;

        case SLIDER_TEMP_MODE_FINE :
            // display slider bar
            tempBar = (U8)(( SliderTemp.SliderVal * SLIDER_BAR_LEVEL) / SLIDER_FULL_RANGE) ;
            DispBar( tempBar );

            // display temperture
            SliderTemp.FineRestVal = calcTempAmount( 
                    SliderTemp.FineVal + SliderTemp.FineRestVal, 
                    &SliderTemp.FineTemp );
            SliderTemp.FineTemp *= TICK_TEMP;

            if( SliderTemp.FineDir == SLIDER_DIR_UP )
            {
                SliderTemp.Temp += SliderTemp.FineTemp;
            }
            else if( SliderTemp.FineDir == SLIDER_DIR_DOWN )
            {
                SliderTemp.Temp -= SliderTemp.FineTemp;
            }
            SliderTemp.Temp = GetTempMinMax( SliderTemp.Temp );

            SmoothUpdateTempDisplay( FALSE, 0, SliderTemp.Temp );
            PlayTempChangeBeep( SliderTemp.Temp );

            // reset variable
            SliderTemp.FineVal = 0;
            SliderTemp.FineTemp = 0;
            break;
    }
}
