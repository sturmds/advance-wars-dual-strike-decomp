#include "types.h"

void Sprite_Queue(int list, int x, int y, const void* data, int attr, int tile);

extern u8 data_ov006_0236c040[];
extern u8 data_ov006_0236c048[];
extern u8 data_ov006_0236c050[];
extern u8 data_ov006_0236c058[];
extern u8 data_ov006_0236c060[];
extern u8 data_ov006_0236c068[];
extern u8 data_ov006_0236c070[];

/* Queues seven sprites on list 0: four at y 0x458 (x 32, 80, 128, 176) and three at
 * y 0x488 (x 32, 80, 128). */
void Ov006_QueueMenuSprites(void) {
  Sprite_Queue(0, 32, 0x458, data_ov006_0236c040, 0, 0);
  Sprite_Queue(0, 80, 0x458, data_ov006_0236c070, 0, 0);
  Sprite_Queue(0, 128, 0x458, data_ov006_0236c068, 0, 0);
  Sprite_Queue(0, 176, 0x458, data_ov006_0236c060, 0, 0);
  Sprite_Queue(0, 32, 0x488, data_ov006_0236c058, 0, 0);
  Sprite_Queue(0, 80, 0x488, data_ov006_0236c050, 0, 0);
  Sprite_Queue(0, 128, 0x488, data_ov006_0236c048, 0, 0);
}
