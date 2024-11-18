#include "hw.h"
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "front.h"

#include "sound.h"

#include "error.h"
#include "display_lib.h"
#include "display_clock.h"
#include "process_display.h"

#include "timer.h"
#include "timer_id.h"

#include "key_event_lib.h"


static void DispOnOff(U8 mu8OnOff);

static U8 SelSound(void);


// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,            2sec,           3sec,               5sec,           Pop,           TS */
    { K_1,        SelSound,      NULL,               NULL, NULL, NULL, NULL },
    { K_2,        SelSound,      NULL,               NULL, NULL, NULL, NULL },
    { K_3,        SelSound,      NULL,               NULL, NULL, NULL, NULL },
    { K_4,        SelSound,      NULL,               NULL, NULL, NULL, NULL },
    { K_5,        SelSound,      NULL,               NULL, NULL, NULL, NULL },
};

U8 IsValidNormalKeyCondition(U32 mu32Key)
{
    return TRUE;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
void* GetNormalKeyEventList(void)
{
    return KeyEventList;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
U8 GetNormalKeyEventListSize(void)
{
    return ( sizeof(KeyEventList) / sizeof(KeyEventList_T) );
}

static U8 SelSound(void)
{
    return SOUND_SELECT;
}

