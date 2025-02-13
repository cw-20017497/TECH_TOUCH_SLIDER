#include "hw.h"

#include "slider_circle_handler.h"  // 슬라이더 핸들러 추가
#include "slider_common_handler.h"
#include "slider.h"
#include "display_slide_amount.h"
#include "display_slide_temp.h"

#include "sound.h"

static U8 SelCirclePushed(void *p);
static U8 SelCircleTouch(void *p);
static U8 SelCircleSlide(void *p);
static U8 SelCirclePop(void *p);
static U8 SelCircleReady(void *p);

// 슬라이더 이벤트 리스트
SliderEventList_T CircleEventList[] =
{
    /* PUSHED,             TOUCH,              SLIDE,              POP,             READY */
    { SelCirclePushed,     SelCircleTouch,     SelCircleSlide,     SelCirclePop,    SelCircleReady, },
};

U8 IsValidSliderCircleCondition(U8 eventFlag)
{
    return TRUE;
}

// 슬라이더 이벤트 리스트 반환
void* GetSliderCircleEventList(void)
{
    return CircleEventList;
}

// 슬라이더 이벤트 리스트 크기 반환
U8 GetSliderCircleEventListSize(void)
{
    return ( sizeof(CircleEventList) / sizeof(SliderEventList_T) );
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
static U8 SelCirclePushed(void *p)
{
    U8 mSound = SOUND_NONE;

    ClearSliderTempMode();

    // READY -> LEVEL MODE
    if( GetSliderAmountMode() == SLIDER_AMOUNT_MODE_READY )
    {
        SetSliderAmountMode( SLIDER_AMOUNT_MODE_LEVEL );
        RefreshSliderAmountMode();
        RefreshSliderAmountLevelMode();
        mSound = SOUND_SELECT;
    }
    // LEVEL -> FINE 
    else if( GetSliderAmountMode() == SLIDER_AMOUNT_MODE_LEVEL 
             && IsExpiredAmountLevelTimer() == TRUE )
    {
        SetSliderAmountMode( SLIDER_AMOUNT_MODE_FINE );
        RefreshSliderAmountMode();
        mSound = SOUND_SELECT;
    }
    else
    {
        RefreshSliderAmountMode();
    }

    SetSliderAmountVal( (( Slider_T *)p)->Val );

    return mSound;
}

static U8 SelCircleTouch(void *p)
{
    RefreshSliderAmountMode();
    RefreshSliderAmountLevelMode();
    return SOUND_NONE;
}

static U8 SelCircleSlide(void *p)
{
    RefreshSliderAmountLevelMode();
    RefreshSliderAmountMode();

    SetSliderAmountVal( (( Slider_T *)p)->Val );
    SetSliderAmountFineVal( (( Slider_T *)p)->SlideAmount, (( Slider_T *)p)->Direction );

    (( Slider_T *)p)->SlideAmount = 0;
    return SOUND_NONE;
}

static U8 SelCirclePop(void *p)
{
    SetSliderAmountAmount( (( Slider_T *)p)->SlideAmount );
    return SOUND_NONE;
}

static U8 SelCircleReady(void *p)
{
    SetSliderAmountAmount( 0 );
    return SOUND_NONE;
}

