#include "types.h"
#include "sdk/os.h"
#include "sdk/tp.h"

#define TOUCH_START_STABILITY_RANGE 20

u16 func_arm7_037fdaf4(void);                    /* V alarms set up? */
void func_arm7_037fdb04(void);                   /* set up V alarms */
void func_arm7_037fd9dc(OSVAlarm *alarm);        /* clear an alarm */
void func_arm7_037fd824(OSVAlarm *alarm, u32 tag);

/* Resets the ARM7 touch panel driver, prepares its auto sampling alarms and
 * leaves the controller ready to report the pen. */
void Touch_ResetDriver_Arm7(void)
{
    int i;

    tpWork.status = TOUCH_SAMPLER_IDLE;
    tpWork.range = TOUCH_START_STABILITY_RANGE;
    tpWork.rangeMin = TOUCH_START_STABILITY_RANGE;
    for (i = 0; i < TOUCH_MSG_MAX_HALFWORDS; i++) {
        tpWork.command[i] = 0;
    }

    if (!func_arm7_037fdaf4()) {
        func_arm7_037fdb04();
    }
    for (i = 0; i < TOUCH_MAX_SAMPLES_PER_FRAME; i++) {
        func_arm7_037fd9dc(&tpWork.vAlarm[i]);
        func_arm7_037fd824(&tpWork.vAlarm[i], TOUCH_LINE_ALARM_ID);
    }

    Touch_WaitBusIdle();
    REG_SPICNT = SPICNT_TP_CONTINUOUS;
    Touch_ShiftOutByte(TOUCH_CTRL_PEN_IRQ_MODE);
    Touch_ShiftOutByte(0);
    REG_SPICNT = SPICNT_TP_1BYTE;
    Touch_ShiftOutByte(0);
}
