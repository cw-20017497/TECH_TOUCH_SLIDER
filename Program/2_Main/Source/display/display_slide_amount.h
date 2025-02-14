#ifndef __DISPLAY_SLIDE_AMOUNT_H__
#define __DISPLAY_SLIDE_AMOUNT_H__

#include "prj_type.h"
#include "display_lib.h"

/* MODE */
#define SLIDER_AMOUNT_MODE_READY  0
#define SLIDER_AMOUNT_MODE_LEVEL  1
#define SLIDER_AMOUNT_MODE_FINE   2

void InitSliderAmount(void);

void SetSliderAmountMode(U8 mode);

U8 GetSliderAmountMode(void);

void SetSliderAmountVal(U16 val);

void SetSliderAmountAmount(U16 amount);

void SetSliderAmountFineVal(U16 val, U8 dir);

U8 IsExpiredAmountLevelTimer(void);

U8 IsExpiredAmountModeTimer(void);

U8 IsExpiredAmountPopTimer(void);

void ResetAmountCirclePosition(void);

void RefreshSliderAmountLevelMode(void);

void ClearSliderAmountMode(void);

void RefreshSliderAmountMode(void);

void ProcessDisplaySliderAmount(void);

#endif /* __DISPLAY_SLIDE_AMOUNT_H__ */

