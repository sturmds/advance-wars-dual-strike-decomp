#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

int abs(int a0);
void MenuDepth_Pop(void);
s8 ScriptSlot_StopSlot(ScriptSlot* target);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gMapScrollSpeeds[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* unk_1c4 points at unk_184 (Screens_InitContexts); this code writes the
 * view through unk_184 itself. */
#define VIEW ((Unk_02080440_1c4*)&GAME->unk_184)

/* One frame of the map scroll script. Moves gMapScrollSpeeds[step] further
 * along the longer axis, the other axis in proportion (16.16 fixed point), and
 * counts step down. After the last entry it puts the view on the target, stops
 * the script and pops the menu depth. Then runs the screen config's callback. */
void MapScroll_Step(MapScrollSlot* slot) {
  int fixedX;
  int along;
  int fixedY;

  if (abs(slot->distX) > abs(slot->distY)) {
    slot->travelled += gMapScrollSpeeds[slot->step];
    along = slot->travelled * (slot->distX > 0 ? 1 : -(slot->distX < 0));
    fixedX = (slot->startX + along) << 16;
    fixedY = ((slot->distY << 16) / slot->distX) * along + (slot->startY << 16);
  } else {
    slot->travelled += gMapScrollSpeeds[slot->step];
    along = slot->travelled * (slot->distY > 0 ? 1 : -(slot->distY < 0));
    fixedX = ((slot->distX << 16) / slot->distY) * along + (slot->startX << 16);
    fixedY = (slot->startY + along) << 16;
  }
  VIEW->scrollX = fixedX / 0x10000;
  VIEW->scrollY = fixedY / 0x10000;
  if (slot->step == 0) {
    VIEW->scrollX = slot->targetX;
    VIEW->scrollY = slot->targetY;
    ScriptSlot_StopSlot((ScriptSlot*)slot);
    MenuDepth_Pop();
  } else {
    slot->step--;
  }
  if (data_027c027c->unk_50[data_027c0284]->callback != NULL) {
    data_027c027c->unk_50[data_027c0284]->callback();
  }
}
