#include "types.h"
#include "sdk/os.h"

/*
 * Compares frame and vcount (now) with the alarm's frame and line. Returns
 * LINE_ALARM_EARLY, LINE_ALARM_ON_TIME or LINE_ALARM_TOO_LATE.
 */
int LineAlarm_ClassifyTiming_Arm7(OSVAlarm *alarm, s32 frame, s32 vcount)
{
    s32 frames = frame - alarm->frame;
    s32 lines = vcount - alarm->fire;

    if (frames < 0 || (frames == 0 && lines < 0)) {
        return LINE_ALARM_EARLY;
    }
    if (lines < 0) {
        lines += LINE_ALARM_FRAME_LINES;
    }
    if (lines <= alarm->delay) {
        return LINE_ALARM_ON_TIME;
    }
    return LINE_ALARM_TOO_LATE;
}
