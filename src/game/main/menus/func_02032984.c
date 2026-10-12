#include "types.h"

void Sound_StartSong(u32 songId);
BOOL CoExp_IsSpecialMission(void);

void func_02032984(void) {
  if (CoExp_IsSpecialMission()) {
    Sound_StartSong(21);
  } else {
    Sound_StartSong(20);
  }
}
