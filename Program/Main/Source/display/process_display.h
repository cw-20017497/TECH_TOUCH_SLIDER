#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"
#include "display_lib.h"

void InitDisplay(void);


/* 초기 FRONT 초기 점멸 완료 상태 */
U8 GetDisplayInit(void);
void StartDisplayInit(void);


void StartDispTimerId(U8 id);
void StopDispTimerId(U8 id);

/* 디스플레이 제어 */
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
