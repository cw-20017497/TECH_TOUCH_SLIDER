#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#define  HAL_KEY_NONE    0x00000000UL

// SLIDER 1-1 ~ 1-5
#define  HAL_KEY_1       0x00000001UL    // SLIDER 1-1 
#define  HAL_KEY_2       0x00000002UL    // SLIDER 1-2
#define  HAL_KEY_3       0x00000004UL    // SLIDER 1-3
#define  HAL_KEY_4       0x00000008UL    // SLIDER 1-4
#define  HAL_KEY_5       0x00000010UL    // SLIDER 1-5

// SLIDER 2-1 ~ 2-5
#define  HAL_KEY_11      0x00000020UL    // SLIDER 2-1
#define  HAL_KEY_12      0x00000040UL    // SLIDER 2-2
#define  HAL_KEY_13      0x00000080UL    // SLIDER 2-3
#define  HAL_KEY_14      0x00000100UL    // SLIDER 2-4
#define  HAL_KEY_15      0x00000200UL    // SLIDER 2-5

// KEY
#define  HAL_KEY_21      0x00000400UL    
#define  HAL_KEY_22      0x00000800UL
#define  HAL_KEY_23      0x00001000UL
#define  HAL_KEY_24      0x00002000UL
#define  HAL_KEY_25      0x00004000UL

#define  HAL_KEY_ALL     0x00007FFFUL


U32 HAL_GetKeyVal(void);

void HAL_SetKeyVal(U32 key);


void HAL_GetSlider(U8 *pBuf);
void HAL_SetSlider(U8 *pBuf);

#endif /* __HAL_KEY_H__ */
