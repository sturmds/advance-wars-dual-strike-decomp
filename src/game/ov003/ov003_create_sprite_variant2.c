#include "types.h"
#include "game/ov003_sound.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"

void* Sprite_CreateTask(int a, const void* b, int c, s16 x, s16 y, int e, int f,
                        int g);
void func_02027aa8(void* sprite, void* a, void* b);
void func_02027aa0(void* sprite, void* a);
extern void* data_ov003_023655e0[];

/* Like Ov003_CreateSprite, with 2 as the third argument of Sprite_CreateTask
 * (Ov003_CreateSprite passes 1) and sound flag 2, which plays quieter. */
void Ov003_CreateSpriteVariant2(int x, int y) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  void* sprite;

  data_ov003_02369c08 |= 2;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  sprite = Sprite_CreateTask(3, data_ov003_023655e0, 2, x, y, 28, 840, 16);
  func_02027aa8(sprite, &data_ov003_02364928->scrollX,
                &data_ov003_02364928->scrollY);
  func_02027aa0(sprite, &data_ov003_02364928->unk_1d6);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
