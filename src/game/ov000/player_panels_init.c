#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/player_panel_cache.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov000_022dabfc[];
extern u8 data_ov000_022db3fc[];

void Screen_SetCurrentGfx(void* state);
void Mem_CopyAuto(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void PlayerPanels_Refresh(void);
void Ov000_LoadObjGfx(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Marks the cached row of every active player as unknown (-1), loads the row
 * graphics and palette on the second screen, then draws the rows. */
void PlayerPanels_Init(void) {
  int i;
  Unk_027c0280* savedGfx;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (STATE->players[i].controller != PLAYER_CONTROLLER_NONE) {
      Ov000_gPlayerPanelCache[i].unitCount = -1;
      Ov000_gPlayerPanelCache[i].properties = -1;
      Ov000_gPlayerPanelCache[i].inGame = -1;
    }
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_CopyAuto(data_ov000_022dabfc, data_027c0280->unk_e4 + 0x4000, 0x800);
  Palette_Load(data_ov000_022db3fc, 32, 352);
  PlayerPanels_Refresh();
  Ov000_LoadObjGfx();
  Screen_SetCurrentGfx(savedGfx);
}
