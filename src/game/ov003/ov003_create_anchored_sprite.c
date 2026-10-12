#include "types.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"

typedef struct Ov003Anchor {
  s16 x;
  s16 y;
  u8 kind;
} Ov003Anchor;

typedef struct Ov003Anchored {
  u8 pad_00[0x60];
  Ov003Anchor anchor;
} Ov003Anchored;

void Ov003_DamageUnitsNear(int a, int x, int y);
void* Sprite_CreateTask(int a, const void* b, int c, s16 x, s16 y, int e, int f,
                        int g);
void func_02027aa8(void* sprite, void* a, void* b);
void func_02027aa0(void* sprite, void* a);
extern u32 data_ov003_02369c08;
extern u8 data_ov003_023654f4[];

/* Creates the sprite for an object that has a position and a kind at 0x60 (the
 * object's purpose is unknown). Calls Ov003_DamageUnitsNear with them first, then
 * makes a sprite there on the first screen and hands it three fields of the
 * overlay state, like Ov003_CreateSprite. */
void Ov003_CreateAnchoredSprite(Ov003Anchored* object) {
  Ov003Anchor* anchor;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  void* sprite;

  data_ov003_02369c08 |= 1;
  anchor = &object->anchor;
  Ov003_DamageUnitsNear(anchor->kind, object->anchor.x, anchor->y);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  sprite = Sprite_CreateTask(3, data_ov003_023654f4, 1, object->anchor.x,
                             anchor->y, 29, 1317, 19);
  func_02027aa8(sprite, &data_ov003_02364928->scrollX,
                &data_ov003_02364928->scrollY);
  func_02027aa0(sprite, &data_ov003_02364928->unk_1d6);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
