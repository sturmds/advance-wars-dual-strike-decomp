#include "sdk/rtc.h"
#include "sdk/os_arm7.h"

/*
 * PXI callback for tag 5: hands a request from the ARM9 to the RTC thread,
 * or answers at once when the command is unknown or a request is running.
 */
void func_arm7_027d58e8(u32 tag, u32 data, BOOL err) {
  u16 command;

  if (err) {
    return;
  }
  command = (data & 0x7f00) >> 8;
  switch (command) {
    case 0x00:
    case 0x01:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
      if (data_arm7_027d81b0.busy) {
        func_arm7_027d58a8(command, RTC_PXI_RESULT_BUSY);
      } else {
        data_arm7_027d81b0.busy = TRUE;
        data_arm7_027d81b0.command = command;
        if (!MsgQueue_PushBack_Arm7(&data_arm7_027d81b0.queue, NULL,
                                    OS_MESSAGE_NOBLOCK)) {
          func_arm7_027d58a8(command, RTC_PXI_RESULT_FATAL_ERROR);
        }
      }
      break;
    default:
      func_arm7_027d58a8(command, RTC_PXI_RESULT_INVALID_COMMAND);
      break;
  }
}
