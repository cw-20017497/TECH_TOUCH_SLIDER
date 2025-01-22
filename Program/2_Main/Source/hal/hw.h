#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"
#include "r_cg_macrodriver.h"


/* HW
 *
 * TIMER12-13 : PWM 
 *  -. DRAIN PUMP OUTPUT
 *
 * TIMER0 
 *  -. BASE TIMER 1ms
 *
 *  TIMER1
 *  -. STEP MOTOR 1ms
 *
 *  TIMER2
 *  -. HEATER OUT ( TRIAC )
 *  -. 8.33ms ( 120hz @ 60hz )
 *
 * PIN INTERRUPT
 *  -. FLOW METER
 *  -. INTP11
 *
 */


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()

/* BUZZER */
#define P_BUZZER_ONOFF              P0.0

#endif /* __HW__H__ */

