#include "types.h"
#include "game/player.h"
#include "game/map_header.h"
#include "game/map_select.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Co_QueueSmallPicture(int coId, void* dest, int paletteSlot);
u8 Map_GetPlayerCount(u16 mapId);
u8 Map_IsFlagSet(u32 mapId);
void Mem_Zero32(u32 value, void* dst, u32 size);

/* War Room map select: on the screen of data_027c0288.unk_04, loads the
 * small pictures of the front and back COs of map slots 2 and up into OBJ
 * VRAM (12 tiles each from tile 16) with palettes 20 and up; an empty CO
 * slot gets cleared tiles. While the selected map is locked (Map_IsFlagSet
 * is 0) the six palettes are cleared, which hides the pictures. */
void MapSelect_LoadEnemyCoPictures(void) {
  /* Needed to match: this declaration order sets the stack slots. */
  int i;
  int playerCount;
  Unk_027c0280* savedState;
  int j;
  int co;

  if (MAP_STATE->gameMode != GAME_MODE_WAR_ROOM) {
    return;
  }
  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  playerCount = Map_GetPlayerCount(SELECTED_MAP_ID());
  for (i = 0; i < playerCount - 1; i++) {
    /* Needed to match: the bound is written as <= count - 1. */
    for (j = 0; j <= PLAYER_CO_COUNT - 1; j++) {
      co = Ov000_gMapHeaders[SELECTED_MAP_ID()].co[i + 2][j];
      if (co != 0) {
        Co_QueueSmallPicture(
            co, data_027c0280->objVram + ((i * 2 + j) * 12 + 16) * 32,
            i * 2 + j + 20);
      } else {
        Mem_Zero32(0, data_027c0280->objVram + ((i * 2 + j) * 12 + 16) * 32,
                   0x180);
      }
      if (!Map_IsFlagSet(SELECTED_MAP_ID())) {
        Mem_Zero32(0, data_027c0280->paletteBuffer + 0x280, 0xc0);
      }
    }
  }
  Screen_SetCurrentGfx(savedState);
}
