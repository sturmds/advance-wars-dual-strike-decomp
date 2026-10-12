#include "types.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"
#include "game/ov003_sprite.h"

Ov003UnitSprite* Sprite_CreateTask(int a, const void* b, int c, s16 x, s16 y,
                                   int e, int f, int g);
void func_02027aa8(void* sprite, void* a, void* b);
void func_02027aa0(void* sprite, void* a);
void func_02027a98(Ov003UnitSprite* sprite,
                   int (*callback)(Ov003UnitSprite* sprite));
int func_ov003_02364300(Ov003UnitSprite* sprite);

extern u8 data_ov003_023654e0[];

/* Creates a sprite on the first screen at (x, y) that points at a unit, and
 * sets func_ov003_02364300 as its callback. */
void Ov003_CreateUnitSprite(int x, int y, Ov003Unit* unit) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  Ov003UnitSprite* sprite;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  sprite = Sprite_CreateTask(3, data_ov003_023654e0, 0, x, y, 28, 0x448, 17);
  func_02027aa8(sprite, &data_ov003_02364928->scrollX,
                &data_ov003_02364928->scrollY);
  func_02027aa0(sprite, &data_ov003_02364928->unk_1d6);
  sprite->unit = unit;
  func_02027a98(sprite, func_ov003_02364300);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
