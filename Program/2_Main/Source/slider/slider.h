#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "hal_slider.h"


#define SLIDER_FULL_RANGE      255
#define SLIDER_HALF_RANGE      127


/* SLIDER ID */
#define  SLIDER_CIRCLE  HAL_SLIDER_1 
#define  SLIDER_BAR     HAL_SLIDER_2 
#define  SLIDER_NUM     2

#define  CLAMPING       0
#define  WARP_AROUND    1

/* DIRECTION */
#define SLIDER_DIR_NONE  0
#define SLIDER_DIR_UP    1
#define SLIDER_DIR_DOWN  2


/* EventFlag Index */
#define  SLIDER_EVENT_PUSHED           0x01
#define  SLIDER_EVENT_TOUCH            0x02
#define  SLIDER_EVENT_SLIDE            0x04
#define  SLIDER_EVENT_POP              0x08
#define  SLIDER_EVENT_READY            0x10
#define  SLIDER_EVENT_ALL              0x1F

typedef struct _slider_
{
    U8   Type;              // CLAMPING or WARP-AROUND
    U16  PrevVal;           // PREV SLIDER POSITION VALUE
    U16  Val;               // SLIDER POSITION VALUE

    U16  SlideAmount;       // move amount
    U16  ResetSlideAmount;  // rested move amount
    U8   Direction;         // up,down

    U8   EventFlag;         // Event Flag..
} Slider_T;


void InitSlider(void);

Slider_T* GetSlider(U8 mu8Id);

U16 GetSliderVal( U8 mu8Id );

U16 GetSliderAmount( U8 mu8Id );

U8 GetSliderDirection( U8 mu8Id );

U8 GetSliderEventFlag(U8 mu8Id );

void SetSliderEventFlag(U8 mu8Id, U8 mu8Event );

void ClearSliderEventFlag(U8 mu8Id, U8 mu8Event );

U8 IsSetSliderEventFlag(U8 mu8Id, U8 mu8Event );

void ProcessScanSlider(void);

#endif /* __SLIDER_H__ */
