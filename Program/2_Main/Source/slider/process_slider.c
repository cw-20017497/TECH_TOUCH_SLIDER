#include "hw.h"

#include "process_slider.h"

#include "slider.h"
#include "slider_common_handler.h"
#include "slider_bar_handler.h"
#include "slider_circle_handler.h"

#include "sound.h"
#include "process_display.h"



/*
 *
 * Param :
 *  -. pValidFunc, : 유효성 검사 함수 포인터 
 *  -. pList, : 슬라이더 이벤트 리스트
 *  -. mu8Size : 슬라이더 이벤트 리스트 크기
 * 
 * Return :
 *  -. FALSE : 슬라이더 입력 처리가 된게 없음.
 *  -. TRUE : 슬라이더 입력 처리가 된게 있음.
 */
static U8 SliderEventHandler(U8 sliderId, SliderValidFun_T pValidFunc, SliderEventList_T *pList, U8 mu8Size)
{
    U8 i;
    U8 mu8Sound;
    U8 eventFlag;
    U8 flag;
    Slider_T *pSlider = NULL;


    if( pList == NULL )
    {
        return TRUE;
    }

    eventFlag = GetSliderEventFlag( sliderId );
    if( pValidFunc( eventFlag ) == FALSE )
    {
        return TRUE;
    }

    pSlider = GetSlider( sliderId);
    for( i = 0; i < SLIDER_ACTION_NUM ; i++ )
    {
        flag = (U8)(1U<<i);
        if( IsSetSliderEventFlag( sliderId, flag) == TRUE )
        {
            ClearSliderEventFlag( sliderId, flag);

            if( pList->actionEvent[i] != NULL )
            {
                mu8Sound = pList->actionEvent[i]( pSlider  );
                Sound( mu8Sound );
            }
        }
    }

    return TRUE;
}

void ProcessSliderEventHandler(void)
{
    SliderValidFun_T    pSliderValidFun = NULL;
    SliderEventList_T  *pSliderEventList = NULL;
    U8                  mu8SliderEventSize = 0;


    if( GetDisplayInit() == FALSE )
    {
        return ;
    }


    // SLIDER CIRCLE 
    pSliderValidFun     = IsValidSliderCircleCondition; 
    pSliderEventList    = (SliderEventList_T *)GetSliderCircleEventList();
    mu8SliderEventSize  = GetSliderCircleEventListSize();
    SliderEventHandler( SLIDER_CIRCLE, pSliderValidFun, pSliderEventList, mu8SliderEventSize );

    // SLIDER BAR 
    pSliderValidFun     = IsValidSliderBarCondition; 
    pSliderEventList    = (SliderEventList_T *)GetSliderBarEventList();
    mu8SliderEventSize  = GetSliderBarEventListSize();
    SliderEventHandler( SLIDER_BAR, pSliderValidFun, pSliderEventList, mu8SliderEventSize );
}

