#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()

/* LED */
#define  P_LED_11                       P0.4
#define  P_LED_12                       P2.0
#define  P_LED_13                       P2.1
#define  P_LED_14                       P2.2
#define  P_LED_15                       P2.3
#define  P_LED_16                       P2.4
#define  P_LED_17                       P2.5

#define  P_LED_21                       P2.6
#define  P_LED_22                       P2.7
#define  P_LED_23                       P14.7
#define  P_LED_24                       P14.6
#define  P_LED_25                       P1.0
#define  P_LED_26                       P1.3
#define  P_LED_27                       P1.4

#define  P_LED_31                       P1.5
#define  P_LED_32                       P1.6
#define  P_LED_33                       P1.7
#define  P_LED_34                       P5.5
#define  P_LED_35                       P5.4
#define  P_LED_36                       P5.3
#define  P_LED_37                       P5.2
                                        
#define  P_LED_41                       P5.1
#define  P_LED_42                       P5.0
#define  P_LED_43                       P3.0
#define  P_LED_44                       P0.5           
#define  P_LED_45                       P0.6            
#define  P_LED_46                       P7.0
#define  P_LED_47                       P7.1

#define  P_LED_51                       P7.2
#define  P_LED_52                       P7.3
#define  P_LED_53                       P7.4
#define  P_LED_54                       P7.5
#define  P_LED_55                       P7.6
#define  P_LED_56                       P7.7
#define  P_LED_57                       P3.1

#endif /* __HW__H__ */

