#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
/* Queues sprite `index` of a loaded sprite file at (x, y); 0 if the file
 * has no such sprite. */
BOOL SpriteFile_Queue(void* set, int index, int arg, int x, int y, int attr,
                      int tile, int list);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236a9d8[];
extern u8 data_ov006_0236a9e0[];
extern u8 data_ov006_0236a9f8[];
extern u8 data_ov006_0236aa68[];

/* Queues the fixed sprites of the second screen: sprites 0 and 1 of /mapedit
 * file 00a (unk_68), each with two more sprites at fixed places. Called
 * by Ov006_UpdateSubScreen. */
void Ov006_QueueScreen1Sprites(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  SpriteFile_Queue(data_ov006_0236d640->unk_68, 0, 0, 88, 166, 0xf2c7, 0, 0);
  Sprite_Queue(0, 80, 136, data_ov006_0236a9d8, 0x3000, 0);
  Sprite_Queue(0, 72, 169, data_ov006_0236a9f8, 0xf000, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_68, 1, 0, 48, 166, 0xf2c7, 0, 0);
  Sprite_Queue(0, 40, 150, data_ov006_0236a9e0, 0x2000, 0);
  Sprite_Queue(0, 32, 169, data_ov006_0236aa68, 0xf000, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
