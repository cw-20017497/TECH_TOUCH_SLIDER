#include "key_event_lib.h"
#include "key.h"
#include "sound.h"
#include "timer.h"
#include "front.h"
#include "process_display.h"



// Continuous Key

//static U8 mu8ContTick    = DEFAULT_CONT_TICK;    // 연속키 
//static U8 mu8ContKeyDown = FALSE;                // 연속키 
//static U8 mu8ContKeyUp   = FALSE;                // 연속키

U8 mu8ContTick    = DEFAULT_CONT_TICK;    // 연속키 
U8 mu8ContKeyDown = FALSE;                // 연속키 
U8 mu8ContKeyUp   = FALSE;                // 연속키

void InitContKey(U8 mu8Tick)
{
    mu8ContTick     = mu8Tick;
    if( mu8ContTick == 0 )
    {
        mu8ContTick = DEFAULT_CONT_TICK;
    }
    mu8ContKeyUp = FALSE;
    mu8ContKeyDown  = FALSE;
}

void ControlContKey( U8 (*pfKeyDown)(void), U8 (*pfKeyUp)(void))
{
    static U8 mu8Delay = 0;


    if( mu8ContKeyDown == TRUE )
    {
        if( mu8Delay == 0 )
        {
            mu8Delay = mu8ContTick;
            if( pfKeyDown != NULL ) 
            {
                pfKeyDown();
            }
        }
        else
        {
            mu8Delay--;
        }
    }
    else if( mu8ContKeyUp == TRUE )
    {
        if( mu8Delay == 0 )
        {
            mu8Delay = mu8ContTick;

            if( pfKeyUp != NULL )
            {
                pfKeyUp();
            }
        }
        else
        {
            mu8Delay--;
        }
    }
    else
    {
        mu8Delay = 0;
    }
}

U8 KeyContDown(void)
{
    mu8ContKeyDown = TRUE;

    return SOUND_SELECT;
}

U8 KeyContUp(void)
{
    mu8ContKeyUp = TRUE;

    return SOUND_SELECT;
}

U8 KeyStopCont(void)
{
    mu8ContKeyUp = FALSE;
    mu8ContKeyDown  = FALSE;

    return SOUND_NONE;
}

