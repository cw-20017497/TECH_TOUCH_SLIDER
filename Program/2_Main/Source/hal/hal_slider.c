#include "hw.h"
#include "hal_slider.h"



U16 HalSlider[ HAL_SLIDER_NUM ];


void HAL_InitSlider(void)
{
    U8 i;

    for( i = 0; i < HAL_SLIDER_NUM; i++ )
    {
        HalSlider[ i ] = 0U;
    }
}

void HAL_SetSliderVal(U8 mu8Id, U16 mu16Val)
{
    HalSlider[ mu8Id ] = mu16Val;
}

U16 HAL_GetSliderVal(U8 mu8Id)
{
    return HalSlider[ mu8Id ];
}

