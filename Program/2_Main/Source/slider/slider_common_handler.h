#ifndef __SLIDER_COMMON_HANDLER_H__
#define __SLIDER_COMMON_HANDLER_H__

#include "prj_type.h"


/* 키 버튼 입력 유효성 조건 검사 함수 인터페이스 */
typedef U8 (*SliderValidFun_T)(U8 eventFlag);

/* 키 이벤트 리스트 인터페이스 */
#define SLIDER_ACTION_PUSHED   0   
#define SLIDER_ACTION_TOUCH    1   
#define SLIDER_ACTION_SLIDE    2   
#define SLIDER_ACTION_POP      3   
#define SLIDER_ACTION_READY    4   
#define SLIDER_ACTION_NUM      5   
typedef U8(*SliderFun_T)(void *p);
typedef struct _slider_event_
{
    SliderFun_T   actionEvent[SLIDER_ACTION_NUM];
} SliderEventList_T;

// SLIDER EVENT LIST SIZE 
typedef U8 (*SliderEventSize_T)(void);

#endif /* __SLIDER_COMMON_HANDLER_H__ */
