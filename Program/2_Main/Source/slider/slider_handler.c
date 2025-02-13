#include "slider_handler.h"
#include "slider_common_handler.h"


// NORMAL  
static SliderValidFun_T    pFunValid          = NULL;    // 유효성 검사 함수
static SliderEventList_T   *pSliderEventList  = NULL;    // 이벤트 리스트 
static U8                  gu8ListSize        = 0;       // 이벤트 리스트 크기


void RegisterSliderEvent( SliderValidFun_T pValid, SliderEventList_T *pList, U8 mu8Size )
{
    pFunValid           = pValid;
    pSliderEventList    = pList;
    gu8ListSize         = mu8Size;
}

U8 IsValidSliderCondition(U8 eventFlag)
{
    U8 mu8Ret = TRUE;

    if( pFunValid != NULL )
    {
        mu8Ret = pFunValid( eventFlag );
    }

    return mu8Ret;
}

void* GetSliderEventList(void)
{
    return pSliderEventList;
}

U8 GetSliderEventListSize(void)
{
    return gu8ListSize;
}

