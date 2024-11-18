#include "hw.h"
#include "hal_key.h"


U32 gu32Key     = 0UL;       // 터치 버튼 

U32 HAL_GetKeyVal(void)
{
    return gu32Key;
}

void HAL_SetKeyVal(U32 key)
{
    gu32Key = key;
}



U8 slider[4] = { 0, 0, 0, 0};
void HAL_GetSlider(U8 *pBuf)
{
    pBuf[0] = slider[0];
    pBuf[1] = slider[1];
    pBuf[2] = slider[2];
    pBuf[3] = slider[3];
}


void HAL_SetSlider(U8 *pBuf)
{
    slider[0] = pBuf[0];
    slider[1] = pBuf[1];
    slider[2] = pBuf[2];
    slider[3] = pBuf[3];
}

