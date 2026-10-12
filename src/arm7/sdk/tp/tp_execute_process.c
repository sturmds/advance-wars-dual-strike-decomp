#include "types.h"
#include "sdk/os.h"
#include "sdk/tp.h"

int func_arm7_037fdb80(void);                     /* disable interrupts */
void func_arm7_037fdb94(int state);               /* restore interrupts */
BOOL func_arm7_03803e68(u32 device);              /* SPI bus free? */
void func_arm7_03803e50(int device);              /* take the SPI bus */
void func_arm7_03803e0c(u32 device);              /* release the SPI bus */
void func_arm7_037fd8ac(OSVAlarm *alarm, s16 count, s16 delay, OSVAlarmHandler handler, void *arg);
void func_arm7_037fd754(u32 tag);                 /* cancel the V alarms with this tag */
void TP_ExecSampling(SPITpData *data, s32 range, u16 *density);
void Touch_TuneStabilityRange(SPITpData *data, u16 density);
void Touch_QueueAutoSample(void *arg);

/* Runs one touch panel job on the SPI thread: a sample (one-shot or from
 * auto sampling), or starting and stopping auto sampling. */
void TP_ExecuteProcess(SerialBusJob *entry)
{
    int i;

    switch (entry->process) {
    case TP_REQUEST_COMMAND_AUTO_SAMPLING:
        if (tpWork.status != TOUCH_SAMPLER_PERIODIC) {
            return;
        }
        /* fall through: an auto sample is taken like a one-shot one */
    case TP_REQUEST_COMMAND_SAMPLING: {
        SPITpData sample;
        u16 density;
        int intr = func_arm7_037fdb80();

        if (!func_arm7_03803e68(SERIAL_OWNER_TOUCH)) {
            func_arm7_037fdb94(intr);
            func_arm7_03803e84(entry->process, TOUCH_REPLY_BUS_TAKEN);
            return;
        }
        func_arm7_03803e50(SERIAL_OWNER_TOUCH);
        func_arm7_037fdb94(intr);

        TP_ExecSampling(&sample, tpWork.range, &density);
        Touch_TuneStabilityRange(&sample, density);
        TOUCH_SHARED_LATEST_LOW = sample.halfs[0];
        TOUCH_SHARED_LATEST_HIGH = sample.halfs[1];

        if (entry->process == TP_REQUEST_COMMAND_SAMPLING) {
            func_arm7_03803e84(entry->process, TOUCH_REPLY_OK);
        } else {
            func_arm7_03803e84(entry->process, entry->arg[0] & 0xff);
        }
        func_arm7_03803e0c(SERIAL_OWNER_TOUCH);
        break;
    }

    case TP_REQUEST_COMMAND_AUTO_ON:
        if (tpWork.status == TOUCH_SAMPLER_ALARMS_PENDING) {
            /* arg[0] alarms per frame, spaced evenly from line arg[1] */
            for (i = 0; i < entry->arg[0]; i++) {
                tpWork.vCount[i] = (entry->arg[1] + i * TOUCH_SCANLINES_PER_FRAME / entry->arg[0]) % TOUCH_SCANLINES_PER_FRAME;
                func_arm7_037fd8ac(&tpWork.vAlarm[i], tpWork.vCount[i], TOUCH_LINE_ALARM_SLACK,
                                   Touch_QueueAutoSample, (void *)i);
            }
            func_arm7_03803e84(entry->process, TOUCH_REPLY_OK);
            tpWork.status = TOUCH_SAMPLER_PERIODIC;
        } else {
            func_arm7_03803e84(entry->process, TOUCH_REPLY_WRONG_STATE);
        }
        break;

    case TP_REQUEST_COMMAND_AUTO_OFF:
        if (tpWork.status == TOUCH_SAMPLER_STOPPING) {
            func_arm7_037fd754(TOUCH_LINE_ALARM_ID);
            func_arm7_03803e84(entry->process, TOUCH_REPLY_OK);
            tpWork.status = TOUCH_SAMPLER_IDLE;
        } else {
            func_arm7_03803e84(entry->process, TOUCH_REPLY_WRONG_STATE);
        }
        break;
    }
}
