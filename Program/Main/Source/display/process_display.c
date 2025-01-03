#include "hw.h"
#include "display.h"
#include "process_display.h"
#include "sound.h"
#include "display_lib.h"
#include "timer.h"
#include "parser.h"
#include "parser_front.h"
#include "hal_led.h"

#include "comm_queue.h"




static void DisplaySelWater(void);
static void DisplaySelHot(void);
static void DisplaySelAmount(void);
static void Display7Segment(void);

static void UpdateTimer(void);
static void ProcessDisplayBoot(void);
static void ProcessDisplayNormalMode(void);


typedef struct _display_
{
    U8 Init;            /* FALSE : 초기화 전, TRUE : 초기화 완료 */
} Display_T;

Display_T   Disp;


void InitDisplay(void)
{
    // TIMER 
    InitDispTimer();

    /* Booting Display - Showing Blink Front Display */
    Disp.Init = FALSE;

    /* TURN OFF ALL LED */
    TurnOffAllLED();
}


U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;

    InitDispTimer();
}

typedef struct _disp_id_time_
{
    U8 id;
    U16 mTime;
} DispTimerVal_T;

DispTimerVal_T DispTimerValTable[] = 
{
    { DISP_TIMER_CONFIG,              TIMER_5SEC }
};
#define SZ_DISP_TIMER_TABLE (sizeof(DispTimerValTable)/sizeof(DispTimerVal_T))

static U16 FindTimeVal(U8 id)
{
    U8 i;

    for( i = 0; i < SZ_DISP_TIMER_TABLE; i++ )
    {
        if( id == DispTimerValTable[ i ].id )
        {
            return DispTimerValTable[ i ].mTime;
        }
    }

    return 0;   // timer 0...
}

// Display Timer start/stop
void StartDispTimerId(U8 id)
{
    U16 mTime;

    mTime = FindTimeVal( id );
    SetDispTimer( id, mTime );
}

void StopDispTimerId(U8 id)
{
    SetDispTimer( id, 0);
}


/* 디스플레이 초기화 표시 */
static void ProcessDisplayBoot(void)
{
    static U8 mu8Count = 5;
    static U8 mu8Toggle = 0;

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        mu8Toggle = !mu8Toggle;

        mu8Count--;
        if( mu8Count == 0 )
        {
            TurnOffAllLED();

            Disp.Init = TRUE;
            mu8Count = 5;
            return;
        }
    }

    if( mu8Toggle == 0 )
    {
        TurnOffAllLED();
    }
    else
    {
        TurnOnAllLED();
    }
}


I16 delta_t[20] = {0};

U8 index = 0;
U8 prev_up_down_val = 0;
#define DIR_UP      2
#define DIR_DOWN    1
#define DIR_NONE    0
static U8 calcDirection(U16 val)
{
    U8 ret = DIR_NONE;

    if( val == 0 )
    {
        prev_up_down_val = val;
        return DIR_NONE;
    }

    if( prev_up_down_val != val 
            && prev_up_down_val == 0 )
    {
        prev_up_down_val = val;

        return DIR_NONE;
    }

    if( prev_up_down_val != val )
    {
        if( prev_up_down_val > val )
        {
            ret = DIR_UP;
        }
        else if( prev_up_down_val < val )
        {
            ret =  DIR_DOWN;
        }

        ////////////////////////////////////////////////
        // for debug, recoding deltea
        delta_t[ index++ ]  = prev_up_down_val - val;
        if( index >= 20 )
        {
            index = 0;
        }
        ///////////////////////////////////////////////
        
        prev_up_down_val = val;
    }

    return ret;
}

#define SPEED_L5        5
#define SPEED_L4        4
#define SPEED_L3        3
#define SPEED_L2        2
#define SPEED_L1        1
#define SPEED_L0        0

I16 prev_speed_val = 0;
I16 delta;
U8 dbg_speed = 0;
U8 dbg_delta = 0;
static U8 calcSpeed(U16 val)
{
    U8 ret = SPEED_L1;
    //I16 delta;

    if( val == 0 )
    {
        prev_speed_val = val;
        return ret;
    }


    if( prev_speed_val != val
            && prev_speed_val == 0 )
    {
        prev_speed_val = (I16)val;

        return SPEED_L1;
    }

    if( prev_speed_val != val )
    {
        delta = prev_speed_val - (I16)val;
        delta = abs( delta );

        if(delta >= 100 ) { ret = SPEED_L5; }
        else if(delta >= 55 ) { ret = SPEED_L4; }
        else if(delta >= 45 ) { ret = SPEED_L3; }
        else if(delta >= 35 ) { ret = SPEED_L2; }
        else if(delta >= 3 ) { ret = SPEED_L1; }
        else { ret = SPEED_L0; }
    }

    prev_speed_val = (I16)val;

    ///////////////////////////////////////
    // for debug......
    dbg_delta = delta;
    dbg_speed = ret;
    ///////////////////////////////////////
    return ret;
}


extern U8 slider[4];
U8 the_disp_level =0;
U8 the_slide = 0;
I8 temp_index = 0;
U8 temp_hot[5] = { 0, 45, 70, 85, 100 };
    
U16 slide_disp_time = 0;        // @10ms..
U8 the_temp = 0;
#define TEMP_TICK   1
#define MAX_TEMP    100
#define MIN_TEMP    40

U8 GetTempTick(U8 speed)
{
    U8 ret_tick = 1;

    if( speed == SPEED_L5 )     { ret_tick = 10; }
    else if( speed == SPEED_L4 ){ ret_tick = 5; }
    else if( speed == SPEED_L3 ){ ret_tick = 4; }
    else if( speed == SPEED_L2 ){ ret_tick = 2; }
    else if( speed == SPEED_L1 ){ ret_tick = 1; }
    else ret_tick = 0;
    
    return ret_tick;
}

U16 effect_time_val = 2;
U16 effect_time = 2;
void DisplayEffectTemp(U16 temp, U8 up_down)
{
    static U16 target_temp = 0;
    static U16 current_temp = 0;



    target_temp = temp;

    if( effect_time != 0 )
    {
        effect_time--;
        DispTemp( current_temp );
        return ;
    }
    else if( target_temp > current_temp )
    {
        current_temp += 1;
        effect_time = effect_time_val;
        DispTemp( current_temp );

    }
    else if( target_temp < current_temp )
    {
        current_temp -= 1;
        effect_time = effect_time_val;
        DispTemp( current_temp );

    }
}

static void ProcessDisplayNormalMode(void)
{
    static U8 prev_slide = 0;
    static U8 prev_disp_level = 0;
    static U8 dir = 0;


    DispLedDuty();

    // reverse side
    if( slider[1] > 0 )
    {
        the_slide = (U8)(256U - slider[1]);
    }
    else
    {
        the_slide = 0;
    }


    // setting slide_disp_time
    if (prev_slide != the_slide)
    {
        if( prev_slide > 0 && the_slide == 0 )
        {
            // pop slide  -> setting micro slide timer
            slide_disp_time = 500; // 5sec @10ms
        }
        else if( prev_slide == 0 && the_slide != 0 )
        {
            if( slide_disp_time == 0 )
            {
                // push slide
                Sound( SOUND_SELECT );
            }
        }

        prev_slide = the_slide;
    }

    // update slide_disp_time
    if( slide_disp_time != 0 
            && the_slide != 0 )
    {
        slide_disp_time = 500; // 5sec @10ms
    }


    if( slide_disp_time != 0 )
    {
        // 표시 대기 시간 카운팅
        slide_disp_time--;
    }

    // 표시 대기 시간이 초과
    if( slide_disp_time == 0 )
    {
        if( the_slide > 192 )       { temp_index = 4; }
        else if( the_slide > 128 )  { temp_index = 3; }
        else if( the_slide > 64 )   { temp_index = 2; }
        else if( the_slide > 0 )    { temp_index = 1; }
        else                        { temp_index = 0; }
    }

    // 표기 대기 시간 중 슬라이드 입력이 있는 경우
    if( slide_disp_time != 0 
            && the_slide != 0 )
    {
        U8 up_down = 0;
        U8 speed = 0;

        speed = calcSpeed( the_slide );
        up_down = calcDirection( the_slide );
        if( up_down  == DIR_DOWN )
        {
            the_temp += GetTempTick( speed );
        }
        else if( up_down  == DIR_UP )
        {
            the_temp -= GetTempTick( speed );
        }
        else if( up_down  == DIR_NONE )
        {
        }

        // check min/max
        if( the_temp < MIN_TEMP )
        {
            the_temp = MIN_TEMP;
        }
        else if( the_temp > MAX_TEMP )
        {
            the_temp = MAX_TEMP;
        }

        DisplayEffectTemp( the_temp, up_down );

        DispBarStack( temp_index * 3 );
    }
    else if( slide_disp_time != 0 && the_slide == 0 )
    {
        // 손을 뗐을 때 이전 값(prev) 업데이트를 위해서 호출
        calcSpeed( the_slide );
        calcDirection( the_slide );
        DisplayEffectTemp( the_temp, 0 );
    }
    else
    {
        // 표시 대기 시간 초과 그리고 입력도 없는 경우
        the_temp = temp_hot[ temp_index ];
        DispTemp( the_temp );
        DispBarStack( temp_index * 3 );
    }

}


#if 0
extern U8 slider[4];
U8 the_disp_level =0;
U8 the_slide = 0;
I8 temp_index = 0;
U8 temp_hot[12] = {
    45, 50, 55, 60, 65,
    70, 75, 80, 85, 90,
    95, 100};
static void ProcessDisplayNormalMode(void)
{
    static U8 prev_slide = 0;
    static U8 prev_disp_level = 0;
    static U8 dir = 0;


    DispLedDuty();

    if( slider[1] > 0 )
    {
        the_slide = (U8)(256U - slider[1]);
    }
    else
    {
        the_slide = 0;
    }

    if( the_slide >= 5 )
    {
        the_disp_level = the_slide / 18;
        the_disp_level++;
        DispBarStack( the_disp_level );

        if( prev_disp_level != the_disp_level)
        {
            if( prev_disp_level < the_disp_level )
            {
                if( dir == 1/* up-ward */)
                {
                    temp_index++;
                }
                dir = 1;
            }
            else
            {
                if( dir == 0 /* down-ward */ )
                {
                    temp_index--;
                }
                dir = 0;
            }
            prev_disp_level = the_disp_level;
        }
    }
    else 
    {
        DispBarStack( 0 );
    }
    
    if( temp_index >= 12 )
    {
        temp_index = 11;
    }
    else if( temp_index <= 0 )
    {
        temp_index = 0;
    }

    DispTemp( temp_hot[ temp_index ] );
}
#endif



U8 dbg_the_disp_update = 0;     // for debug
U8 dbg_update = 0;
static void Update(void)
{
    static U8 delay = 5;

    if( HAL_IsUpdateLed() == TRUE )
    //if( dbg_update != 0 )
    {
     //   dbg_update--;
        if( delay != 0 )
        {
            delay--;
        }
        else
        {
            delay = 5;
            SetCommQueueFront( PKT_FRONT_REQ_LED );

            HAL_UpdateLed();
            dbg_the_disp_update++;  // for debug
        }
    }
}

void ProcessDisplay(void)
{
    UpdateDisplayTimer();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        Update();
        return ;
    }

    /* NORMAL MODE */
    ProcessDisplayNormalMode();
    Update();
}
