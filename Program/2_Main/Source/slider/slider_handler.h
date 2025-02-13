#ifndef __SLIDER_HANDLER_H__
#define __SLIDER_HANDLER_H__

#include "prj_type.h"
#include "slider_common_handler.h"

// 슬라이더 이벤트 리스트 등록 함수
void    RegisterSliderEvent( SliderValidFun_T pValid, SliderEventList_T *pList, U8 mu8Size );

U8      IsValidSliderCondition(U8 eventFlag);
void*   GetSliderEventList(void);
U8      GetSliderEventListSize(void);

#endif /* __SLIDER_HANDLER_H__ */

