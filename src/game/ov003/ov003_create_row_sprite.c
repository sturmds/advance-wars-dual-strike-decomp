#include "types.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"
#include "game/ov003_sprite.h"

Ov003Sprite* Sprite_CreateTask(int a, const void* b, int c, s16 x, s16 y, int e,
                               int f, int g);
void func_02027aa8(void* sprite, void* a, void* b);
void func_02027aa0(void* sprite, void* a);
void func_02027a98(Ov003Sprite* sprite, void (*callback)(Ov003Sprite* sprite));
void Ov003_CreateAnchoredSprite(Ov003Sprite* sprite);

extern u8 data_ov003_023654f4[];

/* Creates a sprite on the first screen at (x, y) that remembers its position
 * and row, and sets Ov003_CreateAnchoredSprite as its callback. */
void Ov003_CreateRowSprite(s16 x, s16 y, u8 row) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  Ov003Sprite* sprite;
  Ov003SpriteSpot* spot;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  sprite = Sprite_CreateTask(3, data_ov003_023654f4, 0, x, y, 14, 0x525, 19);
  func_02027aa8(sprite, &data_ov003_02364928->scrollX,
                &data_ov003_02364928->scrollY);
  func_02027aa0(sprite, &data_ov003_02364928->unk_1d6);
  spot = &sprite->spot;
  spot->x = x;
  spot->y = y;
  spot->row = row;
  func_02027a98(sprite, Ov003_CreateAnchoredSprite);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
