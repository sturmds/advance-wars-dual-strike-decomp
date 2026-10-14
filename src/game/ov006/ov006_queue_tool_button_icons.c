#include "types.h"
#include "game/ov006_cursor_steps.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern Unk_ov006_0235f690* data_ov006_0236d640;

char data_ov006_0236b6bc[] = {
#include "data/ov006/data_ov006_0236b6bc.inc.c"
};
char data_ov006_0236b6b8[] = {
#include "data/ov006/data_ov006_0236b6b8.inc.c"
};
static u8 data_ov006_0236b6c8[] = {
#include "data/ov006/data_ov006_0236b6c8.inc.c"
};
static u8 data_ov006_0236b6d0[] = {
#include "data/ov006/data_ov006_0236b6d0.inc.c"
};
static u8 data_ov006_0236b6d8[] = {
#include "data/ov006/data_ov006_0236b6d8.inc.c"
};
static u8 data_ov006_0236b6c0[] = {
#include "data/ov006/data_ov006_0236b6c0.inc.c"
};
static u8 data_ov006_0236b6e0[] = {
#include "data/ov006/data_ov006_0236b6e0.inc.c"
};
ButtonPos data_ov006_0236b6f4[] = {
#include "data/ov006/data_ov006_0236b6f4.inc.c"
};
ButtonPos data_ov006_0236b6e8[] = {
#include "data/ov006/data_ov006_0236b6e8.inc.c"
};
CursorSteps18 data_ov006_0236b700 = {
#include "data/ov006/data_ov006_0236b700.inc.c"
};
CursorSteps18 data_ov006_0236b714 = {
#include "data/ov006/data_ov006_0236b714.inc.c"
};
CursorSteps24 data_ov006_0236b728 = {
#include "data/ov006/data_ov006_0236b728.inc.c"
};
CursorSteps24 data_ov006_0236b740 = {
#include "data/ov006/data_ov006_0236b740.inc.c"
};
CursorSteps25 data_ov006_0236b774 = {
#include "data/ov006/data_ov006_0236b774.inc.c"
};
CursorSteps25 data_ov006_0236b790 = {
#include "data/ov006/data_ov006_0236b790.inc.c"
};
CursorSteps25 data_ov006_0236b758 = {
#include "data/ov006/data_ov006_0236b758.inc.c"
};
CursorSteps25 data_ov006_0236b7ac = {
#include "data/ov006/data_ov006_0236b7ac.inc.c"
};
CursorSteps26 data_ov006_0236b7c8 = {
#include "data/ov006/data_ov006_0236b7c8.inc.c"
};

#define OV6 data_ov006_0236d640

static inline void QueueSprite(int x, int y, void* data, BOOL affine) {
  Sprite_Queue(0, x & 0x1ff, (y & 0xff) | 0x400 | (affine ? 0x100 : 0), data,
               0x1000, 0);
}

static inline void QueueIcon(ButtonPos* p, void* data, BOOL affine) {
  int x = (p->x + 8) & 0x1ff;
  int y = (p->y + 8) & 0xff;
  QueueSprite(x, y, data, affine);
}

/* Queues the icons of the tool buttons that Ov006_AddToolButtons adds, 8
 * pixels inside each button; the fifth only with flag 1 of unk_02, which
 * also picks the position table. */
void Ov006_QueueToolButtonIcons(void) {
  u16 flags = OV6->unk_02;
  BOOL affine = (flags & 8) ? 1 : 0;
  ButtonPos* pos;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (flags & 1) {
    pos = data_ov006_0236b6e8;
  } else {
    pos = data_ov006_0236b6f4;
  }
  saved0280 = data_027c0280;
  /* Needed to match: these empty tests emit no code but give the saved
   * pointers the registers the original uses. */
  if (!saved0280 && affine) {
  }
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  if (!saved027c && affine) {
  }
  Screen_SetCurrent(data_027c0288.unk_00);
  QueueIcon(&pos[0], data_ov006_0236b6e0, affine);
  QueueIcon(&pos[1], data_ov006_0236b6c8, affine);
  QueueIcon(&pos[2], data_ov006_0236b6d8, affine);
  QueueIcon(&pos[3], data_ov006_0236b6d0, affine);
  if (OV6->unk_02 & 1) {
    QueueIcon(&pos[4], data_ov006_0236b6c0, affine);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
