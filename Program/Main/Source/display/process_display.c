#include "hw.h"
#include "display.h"
#include "process_display.h"
#include "sound.h"
#include "display_lib.h"
#include "timer.h"
#include "parser.h"
#include "parser_front.h"
#include "hal_led.h"




static void DisplaySelWater(void);
static void DisplaySelHot(void);
static void DisplaySelAmount(void);
static void Display7Segment(void);

static void UpdateTimer(void);
static void ProcessDisplayBoot(void);
static void ProcessDisplayNormalMode_Moving(void);
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


U16 dbg_val = 0;
U8 dbg_bar = 0;
U8 dir_bar = 0;
static void ProcessDisplayNormalMode_Moving(void)
{
    DispLedDuty();

    DispTemp(dbg_val);
    if( dbg_val++ > 999 )
    {
        dbg_val = 0;
    }

    DispBarStack( dbg_bar );
    if( dir_bar == 0 )
    {
        dbg_bar++;
        if( dbg_bar >= 14 )
        {
            dir_bar = 1;
        }
    }
    else
    {
        dbg_bar--;
        if( dbg_bar == 1 )
        {
            dir_bar = 0;
        }
    }
}


#define SLIDER_TEMP_END      255
#define SLIDER_TEMP_LEVEL    4

// mode
#define SLIDER_MODE_SELECT  0
#define SLIDER_MODE_SWIFT   1

U8 SliderMode = SLIDER_MODE_SELECT;
U8 LevelValue[SLIDER_TEMP_LEVEL] = { 64, 128, 191, 255 };

static void SliderDispTemp(void)
{
    if( SliderMode == SLIDER_MODE_SELECT )
    {
        if( slider[1] > 0 )
        {
            SliderMode = SLIDER_MODE_SWIFT;
        }

        if( slider[1] 
    }
    else
    {
    }
}

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
        the_slide = 256 - slider[1];
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



static void Update(void)
{
    if( HAL_IsUpdateLed() == TRUE )
    {
        SetCommHeader( COMM_ID_FRONT, PKT_FRONT_REQ_LED );
        StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );

        HAL_UpdateLed();
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
    //ProcessDisplayNormalMode();
    SliderDispTemp();
    Update();
}
