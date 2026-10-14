#include "types.h"
#include "game/player.h"
#include "game/map_header.h"
#include "game/page_strip.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

u8 Map_GetPlayerCount(u16 mapId);
int Survival_GetMapId(int mode, int index);
void Co_LoadSmallPicture(int co, void* dest, int palette);
void Mem_Zero32(u32 value, void* dest, u32 size);

extern Unk_022a7b80 data_022a7b80;
extern Unk_027c0280* data_027c0280;

/* Loads the small pictures of the front and back COs of every computer
 * side (map slots 2 and up) on the current Survival map: 12 tiles each into
 * OBJ VRAM from tile 0, palettes 16 and up. An empty CO slot gets its
 * tiles cleared. */
void SurvivalProgress_LoadEnemyCoPictures(PageStripTask* task) {
  int playerCount;
  int enemy;
  int back; /* 0: front CO, 1: back CO */
  int co;

  playerCount = Map_GetPlayerCount(task->mapId);
  for (enemy = 0; enemy < playerCount - 1; enemy++) {
    /* Needed to match: the bound is written as <= count - 1. */
    for (back = 0; back <= PLAYER_CO_COUNT - 1; back++) {
      co =
          Ov000_gMapHeaders[Survival_GetMapId(data_022a7b80.mode, task->cursor)]
              .co[enemy + 2][back];
      if (co != 0) {
        Co_LoadSmallPicture(
            co, data_027c0280->objVram + (((enemy * 2 + back) * 12) << 5),
            enemy * 2 + back + 16);
      } else {
        Mem_Zero32(0, data_027c0280->objVram + (((enemy * 2 + back) * 12) << 5),
                   12 * 32);
      }
    }
  }
}
