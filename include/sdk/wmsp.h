#ifndef SDK_WMSP_H
#define SDK_WMSP_H

#include "types.h"

/* ARM7 side of WM: a command for the wireless (WL) firmware.
   words[] holds the arguments, then the reply header at words[index]:
   the argument count, then the command id. WL fills in the reply. */
typedef struct WlCommand {
    u16 reserved[6];
    u16 length;
    u16 index;
    u16 words[1];
} WlCommand;

/* Copies halfwords (ARM7's MI_CpuCopy16-like helper). */
void func_arm7_037fde40(const void *src, void *dest, int size);

/* Sends the command to the firmware. */
void func_arm7_027c0560(WlCommand *command);

#endif
