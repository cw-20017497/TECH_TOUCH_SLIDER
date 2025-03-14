#include "hw.h"
#include "slider.h"

#include <string.h>
#include <stdlib.h>



Slider_T   Slider[ SLIDER_NUM ];


void InitSlider(void)
{
    MEMSET((void __FAR *)&Slider, 0, sizeof( Slider ) );

    Slider[ SLIDER_BAR ].Type    = CLAMPING;
    Slider[ SLIDER_CIRCLE ].Type = WRAP_AROUND;
}


Slider_T* GetSlider(U8 mu8Id)
{
    return &Slider[ mu8Id ];
}


U16 GetSliderVal( U8 mu8Id )
{
    return Slider[ mu8Id ].Val;
}

U16 GetSliderAmount( U8 mu8Id )
{
    return Slider[ mu8Id ].SlideAmount;
}


U8 GetSliderDirection( U8 mu8Id )
{
    return Slider[ mu8Id ].Direction;
}


U8 GetSliderEventFlag(U8 mu8Id )
{
    return Slider[mu8Id].EventFlag;
}

void SetSliderEventFlag(U8 mu8Id, U8 mu8Event )
{
    Slider[mu8Id].EventFlag |= mu8Event;
}

void ClearSliderEventFlag(U8 mu8Id, U8 mu8Event )
{
    Slider[mu8Id].EventFlag &= ~mu8Event;
}

U8 IsSetSliderEventFlag(U8 mu8Id, U8 mu8Event )
{
    if( (Slider[mu8Id].EventFlag & mu8Event) ==  mu8Event )
    {
        return TRUE;
    }

    return FALSE;
}


U16 dbg_amount[30];

static U16 CalcSliderAmount(U16 prevVal, U16 curVal, U8 Type )
{
    I16 amount = 0;
    static i = 0;

    if( prevVal != curVal )
    {
        amount = (I16)prevVal - (I16)curVal;

        if( Type == WRAP_AROUND )
        {
            if( amount > SLIDER_HALF_RANGE )
            {
                amount -= SLIDER_FULL_RANGE;
            }
            else if( amount < -SLIDER_HALF_RANGE )
            {
                amount += SLIDER_FULL_RANGE;
            }
        }

        amount = abs( amount );

        /////////////////////////////////////////
        {
            dbg_amount[ i++] =  amount;
            if( i >= 30 )
            {
                i = 0;
            }
        }
        /////////////////////////////////////////
    }

    return (U16)amount;
}


//static U8 CalcSliderDirection(U16 prevVal, U16 curVal)
//{
//    if( prevVal < curVal )
//    {
//        return SLIDER_DIR_UP;
//    }
//
//    return SLIDER_DIR_DOWN;
//}

static void CalcSliderDirection(U16 prevVal, U16 curVal, U8 type, U8 *pDir )
{
    I16 amount = 0;


    if( type == WRAP_AROUND )
    {
        amount = (I16)prevVal - (I16)curVal;

        if( amount > SLIDER_HALF_RANGE 
                ||  amount < -SLIDER_HALF_RANGE )
        {
            return ;
        }
    }

    if( prevVal < curVal )
    {
        *pDir = SLIDER_DIR_UP;
    }
    else
    {
        *pDir = SLIDER_DIR_DOWN;
    }
}


void ProcessScanSlider(void)
{
    U8 i; 
    Slider_T *p;

    
    for( i = 0; i < SLIDER_NUM; i++ )
    {

        p = &Slider[i];
        p->Val = HAL_GetSliderVal( i );

        if( p->PrevVal != p->Val )
        {
            if( p->PrevVal == 0 )
            {
                SetSliderEventFlag( i, SLIDER_EVENT_PUSHED );

            }
            else if( p->Val == 0 )
            {
                SetSliderEventFlag( i, SLIDER_EVENT_POP );
            }
            else
            {
                p->SlideAmount = CalcSliderAmount( p->PrevVal, p->Val, p->Type );
                //p->Direction   = CalcSliderDirection( p->PrevVal, p->Val );
                CalcSliderDirection( p->PrevVal, p->Val, p->Type, &p->Direction ); 

                SetSliderEventFlag( i, SLIDER_EVENT_SLIDE );
            }

            p->PrevVal = p->Val;
        }
        else 
        {
            if( p->Val == 0 )
            {
//                SetSliderEventFlag( i, SLIDER_EVENT_READY );
            }
            else if( p->Val != 0 )
            {
                SetSliderEventFlag( i, SLIDER_EVENT_TOUCH );
            }

            p->Direction = SLIDER_DIR_NONE;    // init direction
        }
    }
}
