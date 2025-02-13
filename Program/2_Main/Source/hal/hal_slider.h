#ifndef __HAL_SLIDER_H__
#define __HAL_SLIDER_H__

#define HAL_SLIDER_1        0
#define HAL_SLIDER_2        1
#define HAL_SLIDER_NUM      2

void HAL_InitSlider(void);

void HAL_SetSliderVal(U8 mu8Id, U16 mu16Val);
U16 HAL_GetSliderVal(U8 mu8Id);

#endif /* __HAL_SLIDER_H__ */
