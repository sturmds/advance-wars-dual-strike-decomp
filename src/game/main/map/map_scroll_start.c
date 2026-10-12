#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/script_slot.h"

BOOL func_020f5848(void);
BOOL MapView_FindScroll(int* x, int* y, BOOL centre);
void MenuDepth_Push(void);
ScriptSlot* ScriptSlot_Start(const void* script, int arg);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const EventScriptCommand gMapScrollScript[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Starts the map scroll script towards tile (x, y), unless one is running or
 * the view already shows the tile (see MapView_FindScroll for centre). It
 * clears the view's scroll flags and holds the menu depth. */
void MapScroll_Start(int x, int y, BOOL centre) {
  MapScrollSlot* slot;

  if (func_020f5848()) {
    return;
  }
  if (!MapView_FindScroll(&x, &y, centre)) {
    return;
  }
  GAME->unk_1c4->scrollFlags = 0;
  slot = (MapScrollSlot*)ScriptSlot_Start(gMapScrollScript, 0);
  slot->targetX = x * 16;
  slot->targetY = y * 16;
  MenuDepth_Push();
}
