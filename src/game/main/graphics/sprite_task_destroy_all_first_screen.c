#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Task_DestroyByScript(const void* script);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 gSpriteTaskScript[];

/* Destroys the sprite tasks of the first screen (data_027c0288.unk_00). */
void SpriteTask_DestroyAllFirstScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Task_DestroyByScript(gSpriteTaskScript);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
