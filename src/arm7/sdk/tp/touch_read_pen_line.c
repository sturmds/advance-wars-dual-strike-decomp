#include "types.h"
#include "sdk/tp.h"

/* ARM7 key input register; bit 6 reads 0 while the pen is down. */
#define REG_EXTKEYIN (*(volatile u16*)0x04000136)
#define EXTKEYIN_PEN_UP 0x0040

#define RCNT_GPIO 0x8000 /* serial port pins as general purpose I/O */

void func_arm7_037fe3e0(int mode); /* set RCNT mode */

/* Puts the controller into pen-detect mode and sends one empty byte. */
static inline void Touch_EnterPenDetectMode(void) {
  REG_SPICNT = SPICNT_TP_CONTINUOUS;
  Touch_ShiftOutByte(TOUCH_CTRL_PEN_IRQ_MODE);
  Touch_ShiftOutByte(0);
  REG_SPICNT = SPICNT_TP_1BYTE;
  Touch_ShiftOutByte(0);
}

/* Reads the pen-down line. If the pen was down last time and the line now
 * reads up, looks once more: down then means the up reading may have been
 * noise (PEN_UP_GLITCH). */
u32 Touch_ReadPenLine(void) {
  func_arm7_037fe3e0(RCNT_GPIO);

  Touch_WaitBusIdle();
  Touch_EnterPenDetectMode();

  if (!tpLastTouch) {
    return !(REG_EXTKEYIN & EXTKEYIN_PEN_UP);
  }
  if (!(REG_EXTKEYIN & EXTKEYIN_PEN_UP)) {
    return TP_TOUCH_ON;
  }

  Touch_EnterPenDetectMode();
  if (!(REG_EXTKEYIN & EXTKEYIN_PEN_UP)) {
    return PEN_UP_GLITCH;
  } else {
    return TP_TOUCH_OFF;
  }
}
