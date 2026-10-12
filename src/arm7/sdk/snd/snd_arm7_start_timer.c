#include "sdk/snd.h"
#include "sdk/os_arm7.h"

#define AUDIO_CH_CTRL_TOP_BYTE(channel) (*(u8*)(0x04000403 + ((channel) << 4)))
#define AUDIO_RECORD_PAIR_CNT (*(u16*)0x04000508)

void SndArm7_StartTimer(u32 channelMask, u32 captureMask, u32 alarmMask,
                        u32 flags) {
  int irqState = Cpu_MaskIrq_Arm7();
  int bit;

  for (bit = 0; bit < 16 && channelMask != 0; bit++, channelMask >>= 1) {
    if (channelMask & 1) {
      AUDIO_CH_CTRL_TOP_BYTE(bit) |= 0x80;
    }
  }
  if (captureMask & 1) {
    if (!(captureMask & 2)) {
      AUDIO_RECORD0_CNT |= 0x80;
    } else {
      AUDIO_RECORD_PAIR_CNT |= 0x8080;
    }
  } else if (captureMask & 2) {
    AUDIO_RECORD1_CNT |= 0x80;
  }
  for (bit = 0; bit < SOUND_TIMER_SLOTS_ARM7 && alarmMask != 0;
       bit++, alarmMask >>= 1) {
    if (alarmMask & 1) {
      SndArm7_StartAlarm(bit);
    }
  }
  Cpu_RestoreIrqBit_Arm7(irqState);
  SndArm7_UpdateSharedWork();
}
