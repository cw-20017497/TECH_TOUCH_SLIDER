#include "hw.h"

#include "slider_bar_handler.h"  // 슬라이더 핸들러 추가
#include "slider_common_handler.h"
#include "slider.h"
#include "display_slide_temp.h"
#include "display_slide_amount.h"

#include "sound.h"



static U8 SelBarPushed(void *p);
static U8 SelBarTouch(void *p);
static U8 SelBarSlide(void *p);
static U8 SelBarPop(void *p);
static U8 SelBarReady(void *p);


// 슬라이더 이벤트 리스트
SliderEventList_T BarEventList[] =
{
    /* PUSHED,          TOUCH,             SLIDE,             POP,             READY */
    { SelBarPushed,     SelBarTouch,       SelBarSlide,       SelBarPop,       SelBarReady, },
};

U8 IsValidSliderBarCondition(U8 eventFlag)
{
    return TRUE;
}

// 슬라이더 이벤트 리스트 반환
void* GetSliderBarEventList(void)
{
    return BarEventList;
}

// 슬라이더 이벤트 리스트 크기 반환
U8 GetSliderBarEventListSize(void)
{
    return ( sizeof(BarEventList) / sizeof(SliderEventList_T) );
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
static U8 SelBarPushed(void *p)
{
    U8 mSound = SOUND_NONE;

    ClearSliderAmountMode();

    // READY -> LEVEL MODE
    if( GetSliderTempMode() == SLIDER_TEMP_MODE_READY )
    {
        SetSliderTempMode( SLIDER_TEMP_MODE_LEVEL );
        RefreshSliderTempMode();
        RefreshSliderTempLevelMode();
        mSound = SOUND_SELECT;
    }
    // LEVLE -> FINE 
    else if( GetSliderTempMode() == SLIDER_TEMP_MODE_LEVEL 
            && IsExpiredTempLevelTimer() == TRUE )
    {
        SetSliderTempMode( SLIDER_TEMP_MODE_FINE );
        RefreshSliderTempMode();
        mSound = SOUND_SELECT;
    }
    else
    {
        RefreshSliderTempMode();
    }

    SetSliderTempVal( (( Slider_T *)p)->Val );

    return mSound;
}

static U8 SelBarTouch(void *p)
{
    RefreshSliderTempMode();
    RefreshSliderTempLevelMode();
    return SOUND_NONE;
}

static U8 SelBarSlide(void *p)
{
    RefreshSliderTempLevelMode();
    RefreshSliderTempMode();

    SetSliderTempVal( (( Slider_T *)p)->Val );
    SetSliderTempFineVal( (( Slider_T *)p)->SlideAmount, (( Slider_T *)p)->Direction);

    (( Slider_T *)p)->SlideAmount = 0;
    return SOUND_NONE;
}

static U8 SelBarPop(void *p)
{
    SetSliderTempAmount( (( Slider_T *)p)->SlideAmount );
    return SOUND_NONE;
}

static U8 SelBarReady(void *p)
{
    SetSliderTempAmount( 0 );
    return SOUND_NONE;
}
