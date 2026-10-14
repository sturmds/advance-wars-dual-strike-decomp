#include "types.h"
#include "game/ov003_sound.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"

typedef struct Ov003CallbackSprite {
  u8 pad_00[0x60];
  int userValue;
} Ov003CallbackSprite;

void* Sprite_CreateTask(int a, const void* b, int c, s16 x, s16 y, int e, int f,
                        int g);
void func_02027aa8(void* sprite, void* a, void* b);
void func_02027aa0(void* sprite, void* a);
void func_02027a98(void* sprite, void (*callback)(void*));
void Ov003_SpriteCallback(void* sprite);
extern void* data_ov003_023655e0[];

/* Like Ov003_CreateSprite, but stores value at 0x60 of the sprite and sets
 * Ov003_SpriteCallback as its callback (which passes the value to
 * Ov003Unit_Remove). */
void Ov003_CreateSpriteWithCallback(int x, int y, int value) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  Ov003CallbackSprite* sprite;

  data_ov003_02369c08 |= 1;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  sprite = Sprite_CreateTask(3, data_ov003_023655e0, 1, x, y, 28, 840, 16);
  func_02027aa8(sprite, &data_ov003_02364928->scrollX,
                &data_ov003_02364928->scrollY);
  func_02027aa0(sprite, &data_ov003_02364928->unk_1d6);
  sprite->userValue = value;
  func_02027a98(sprite, Ov003_SpriteCallback);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
