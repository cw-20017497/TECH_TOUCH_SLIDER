#ifndef __DISPLAY_SLIDE_TEMP_H__
#define __DISPLAY_SLIDE_TEMP_H__

#include "prj_type.h"
#include "display_lib.h"


/* MODE */
#define SLIDER_TEMP_MODE_READY  0
#define SLIDER_TEMP_MODE_LEVEL  1
#define SLIDER_TEMP_MODE_FINE   2


void InitSliderTemp(void);

void SetSliderTempMode(U8 mode);

U8 GetSliderTempMode(void);

void SetSliderTempVal(U16 val);

void SetSliderTempAmount(U16 amount);

void SetSliderTempFineVal(U16 val, U8 dir);

U8 IsExpiredTempLevelTimer(void);

U8 IsExpiredTempModeTimer(void);

void ClearSliderTempMode(void);

void ResetTempBarPosition(void);

void RefreshSliderTempLevelMode(void);

void RefreshSliderTempMode(void);

void ProcessDisplaySliderTemp(void);

#endif /* __DISPLAY_SLIDE_TEMP_H_ */
