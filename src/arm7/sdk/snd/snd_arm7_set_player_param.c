#include "sdk/snd_seq.h"

/* Stores value (size bytes: 1, 2 or 4) at a byte offset of a player. */
void SndArm7_SetPlayerParam(int playerNo, int offset, u32 value, int size) {
  SndPlayer* player = &data_arm7_038085d0[playerNo];

  switch (size) {
    case 1:
      *(u8*)((u8*)player + offset) = value;
      break;
    case 2:
      *(u16*)((u8*)player + offset) = value;
      break;
    case 4:
      *(u32*)((u8*)player + offset) = value;
      break;
  }
}
