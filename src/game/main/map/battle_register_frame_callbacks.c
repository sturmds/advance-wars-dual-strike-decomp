#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 func_020d1be0[];
extern u8 WeatherFx_UpdateFirstScreen[];
extern u8 MainLoop_MapFrame[];

#define HOOKS (data_027c027c->unk_50[data_027c0284])

s8 CallbackSlots_Add(const void* descriptor);
void MainLoop_SetFrameCallback(const void* descriptor);
void func_020c0734(void);
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);

/* Battle start step: runs the screen config's unk_38 hook, registers the
 * frame callbacks, and adds a sprite task when the current state is data_027c0288.unk_04. */
void Battle_RegisterFrameCallbacks(void) {
  if (HOOKS->unk_38 != NULL) {
    HOOKS->unk_38();
  }
  if (data_027c027c == data_027c0288.unk_00) {
    CallbackSlots_Add(WeatherFx_UpdateFirstScreen);
  } else {
    CallbackSlots_Add(func_020d1be0);
  }
  MainLoop_SetFrameCallback(MainLoop_MapFrame);
  func_020c0734();
  if (data_027c027c != data_027c0288.unk_04) {
    return;
  }
  if (Link_IsActive() != 1) {
    return;
  }
  SpriteTask_Create(4, 4, 0x3a0, 15);
}
