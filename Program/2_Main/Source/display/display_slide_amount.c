#include "hw.h"

#include "display_slide_amount.h"

#include "slider.h"
#include "display.h"
#include "sound.h"

#define SLIDER_BAR_LEVEL    12
#define DEFAULT_AMOUNT      120
#define TICK_AMOUNT         10

/* MODE TIMER */
#define SLIDER_LEVEL_TIMER       30  /* 300ms@10ms*/
#define SLIDER_MODE_TIMER       500 /* 5sec@10ms*/

/* LEVEL MODE MIN/MAX */
#define MIN_AMOUNT        100
#define MAX_AMOUNT        990

#define UPDATE_DELAY    2

typedef struct _amount_data_
{
    U16 SlideVal;
    U16 Amount;
} AmountData_T;

#define MAX_AMOUNT_DATA_NUM  4
AmountData_T amountDataList[ MAX_AMOUNT_DATA_NUM ] = 
{
    { 0,    120 },
    { 64,   250 },
    { 128,  500 },
    { 192,  700 }
};

typedef struct _slide_amount_
{
    U8 Mode;
    U16 ModeTimer;
    U16 LevelTimer;

    U16 Amount;              // formerly "Temp"
    U16 SliderVal;
    U16 SliderAmount;
    U16 SliderRemainAmount;

    /* FINE TOUCH */
    U8 FineDir;
    U16 FineVal;
    U16 FineRestVal;
    U16 FineAmount;          // formerly "FineTemp"
} DispSliderAmount_T;

DispSliderAmount_T SliderAmount;

void InitSliderAmount(void)
{
    SliderAmount.Mode = SLIDER_AMOUNT_MODE_READY;
    SliderAmount.Amount = DEFAULT_AMOUNT;
}

void SetSliderAmountMode(U8 mode)
{
    SliderAmount.Mode = mode;
}

U8 GetSliderAmountMode(void)
{
    return SliderAmount.Mode;
}

void SetSliderAmountVal(U16 val)
{
    SliderAmount.SliderVal = val;
}

void SetSliderAmountAmount(U16 amount)
{
    SliderAmount.SliderAmount = amount;
}

void SetSliderAmountFineVal(U16 val, U8 dir)
{
    SliderAmount.FineVal = val;
    SliderAmount.FineDir = dir;
}

U8 IsExpiredAmountLevelTimer(void)
{
    if( SliderAmount.LevelTimer == 0 )
    {
        return TRUE;
    }
    return FALSE;
}

U8 IsExpiredAmountModeTimer(void)
{
    if( SliderAmount.ModeTimer == 0 )
    {
        return TRUE;
    }
    return FALSE;
}

void RefreshSliderAmountLevelMode(void)
{
    SliderAmount.LevelTimer = SLIDER_LEVEL_TIMER;
}

void ClearSliderAmountMode(void)
{
    SliderAmount.ModeTimer = 0;
}

void RefreshSliderAmountMode(void)
{
    SliderAmount.ModeTimer = SLIDER_MODE_TIMER;
}

static U16 GetAmountMinMax(U16 amount)
{
    if( amount < MIN_AMOUNT )
    {
        amount = MIN_AMOUNT;
    }
    else if( amount > MAX_AMOUNT )
    {
        amount = MAX_AMOUNT;
    }
    return amount;
}

static U8 GetAmountIndexFromSlide(U16 slide)
{
    U8 retIndex = 0;

    if( slide > amountDataList[3].SlideVal )       { retIndex = 3; }
    else if( slide > amountDataList[2].SlideVal )  { retIndex = 2; }
    else if( slide > amountDataList[1].SlideVal )  { retIndex = 1; }
    else                                           { retIndex = 0; }

    return retIndex;
}

static U16 GetAmountFromSlide(U16 slide)
{
    U8 index;
    index = GetAmountIndexFromSlide(slide);
    return amountDataList[index].Amount;
}

static U16 calcAmountAmount(U16 Val, U16 *pAmountAmount)
{
    U16 restAmount;
    *pAmountAmount = Val / SLIDER_BAR_LEVEL;
    restAmount = Val % (SLIDER_BAR_LEVEL * (*pAmountAmount));
    return restAmount;
}


static void SmoothUpdateAmountDisplay(U8 isInit, U16 initAmount, U16 targetAmount)
{
    static U16 currentAmount = 0;
    static U16 updateDelay = UPDATE_DELAY;

    // setting current amount 
    if( isInit == TRUE )
    {
        currentAmount = initAmount;
        return;
    }

    if (updateDelay > 0)
    {
        updateDelay--;
    }
    else if (currentAmount != targetAmount)
    {
        currentAmount += (targetAmount > currentAmount) ? TICK_AMOUNT : -TICK_AMOUNT;
        updateDelay = UPDATE_DELAY;
    }

    DispAmount(currentAmount);
}

static void PlayAmountChangeBeep(U16 currentAmount)
{
    static U16 prevAmount;

    if( prevAmount != currentAmount )
    {
        prevAmount = currentAmount;
        Sound(SOUND_SELECT);
    }
}

void ProcessDisplaySliderAmount(void)
{
    U8 amountBar = 0;
    U8 direction = 0;

    // Reset Mode by timer
    if( SliderAmount.ModeTimer != 0 )
    {
        SliderAmount.ModeTimer--;
    }
    else
    {
        if( SliderAmount.Mode == SLIDER_AMOUNT_MODE_LEVEL )
        {
            SliderAmount.Mode = SLIDER_AMOUNT_MODE_READY;
        }
        else if( SliderAmount.Mode == SLIDER_AMOUNT_MODE_FINE )
        {
            SliderAmount.Mode = SLIDER_AMOUNT_MODE_READY;
        }
    }

    if( SliderAmount.LevelTimer != 0 )
    {
        SliderAmount.LevelTimer--;
    }
    else
    {
        if( SliderAmount.Mode == SLIDER_AMOUNT_MODE_LEVEL )
        {
            SliderAmount.Mode = SLIDER_AMOUNT_MODE_FINE;
        }
    }

    // display mode
    switch( SliderAmount.Mode )
    {
        case SLIDER_AMOUNT_MODE_READY:
            if( SliderAmount.ModeTimer == 0 )
            {
                TurnOffAllLED();
                SliderAmount.Amount = DEFAULT_AMOUNT;
                DispAmount(SliderAmount.Amount);
            }
            break;

        case SLIDER_AMOUNT_MODE_LEVEL:
            // display slider bar
            amountBar = GetAmountIndexFromSlide(SliderAmount.SliderVal);
            if( amountBar == 0 ) amountBar = 3;
            else if( amountBar == 1 ) amountBar = 6;
            else if( amountBar == 2 ) amountBar = 9;
            else if( amountBar == 3 ) amountBar = 12;
            DispCircleStack(amountBar);

            // display amount
            SliderAmount.Amount = GetAmountFromSlide(SliderAmount.SliderVal);
            DispAmount(SliderAmount.Amount);

            // reset variable
            SliderAmount.FineRestVal = 0;
            SmoothUpdateAmountDisplay(TRUE, SliderAmount.Amount, 0);

            break;

        case SLIDER_AMOUNT_MODE_FINE:
            // display slider bar
            amountBar = (U8)(( SliderAmount.SliderVal * SLIDER_BAR_LEVEL) / SLIDER_FULL_RANGE) ;
            DispCircle(amountBar);

            // display amount
            SliderAmount.FineRestVal = calcAmountAmount(
                    SliderAmount.FineVal + SliderAmount.FineRestVal,
                    &SliderAmount.FineAmount );

            SliderAmount.FineAmount *= TICK_AMOUNT;

            if( SliderAmount.FineDir == SLIDER_DIR_UP )
            {
                SliderAmount.Amount += SliderAmount.FineAmount;
            }
            else if( SliderAmount.FineDir == SLIDER_DIR_DOWN )
            {
                SliderAmount.Amount -= SliderAmount.FineAmount;
            }

            SliderAmount.Amount = GetAmountMinMax(SliderAmount.Amount);

            SmoothUpdateAmountDisplay(FALSE, 0, SliderAmount.Amount);
            PlayAmountChangeBeep(SliderAmount.Amount);

            // reset variable
            SliderAmount.FineVal = 0;
            SliderAmount.FineAmount = 0;
            break;
    }
}

