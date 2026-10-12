#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void SpriteFile_Queue(void* cells, int cell, int arg2, int x, int y, int tile,
                      int arg6, int arg7);

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Runs each frame of the second page of Ov006_UpdateTextPages: queues sprite
 * cells 0-4 of /mapedit/00c at x 40, 16 pixels apart, next to the text lines.
 * Tile 0x24a is where /mapedit/00b was unpacked (sprite VRAM + 0x4940). */
void Ov006_QueuePage2Icons(void) {
  SpriteFile_Queue(data_ov006_0236d640->unk_74, 0, 0, 40, 48, 0x24a, 0, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_74, 1, 0, 40, 64, 0x24a, 0, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_74, 2, 0, 40, 80, 0x24a, 0, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_74, 3, 0, 40, 96, 0x24a, 0, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_74, 4, 0, 40, 112, 0x24a, 0, 0);
}
