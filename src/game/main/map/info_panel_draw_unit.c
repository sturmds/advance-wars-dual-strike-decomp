#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/info_panel.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;

extern void* data_021663b4[]; /* [owner - 1]: unit sprite shape with a badge */
extern void* data_021663c8[]; /* [owner - 1]: unit sprite shape without one */

BOOL UnitType_HidesCargo(int type, int viewer, int owner);
u8* UnitBadges_GetTiles(void);
int UnitIcons_GetTileCount(void);
u8* UnitIcons_GetTiles(void);
int Player_GetUnitIconTile(int player, int unitType);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define GFX data_027c0280

/* Object tiles of slot 0's sprite; each slot takes 4 tiles. */
#define UNIT_TILE 840
/* The badge tile sits in the sprite's second row, on the right for an odd
 * owner. */
#define BADGE_TILE (UNIT_TILE + 2)

/* Badge tiles, numbered as if UnitBadges_GetTiles's sheet followed the
 * UnitIcons_GetTileCount() (136) tiles of UnitIcons_GetTiles's icon sheet. */
#define BADGE_CARRYING 149
#define BADGE_CAPTURE 153
#define BADGE_DIVED 157
#define BADGE_HIDDEN 161

#define QUEUE_BADGE(badge, slot, owner)                                  \
  TransferQueue_Add(                                                     \
      UnitBadges_GetTiles() + ((badge) - UnitIcons_GetTileCount()) * 32, \
      GFX->objVram + ((owner & 1) + (BADGE_TILE + (slot) * 4)) * 32, 32)

/* Draws a unit icon in the info panel with at most one badge layer per
 * condition: hidden cargo replaces the others, else carrying, capturing
 * and dived each queue their badge. A sprite with a badge uses the
 * data_021663b4 shape. */
void InfoPanel_DrawUnit(s16 x, s16 y, int type, int owner, u8 carrying,
                        u8 captureProgress, u8 flags, int slot) {
  int badges;
  int tile;

  if (carrying) {
    carrying = TRUE;
  }
  badges = 0;
  if (UnitType_HidesCargo((u8)type, STATE->currentPlayer, (u16)owner)) {
    QUEUE_BADGE(BADGE_HIDDEN, slot, owner);
    badges++;
  } else {
    if (carrying) {
      QUEUE_BADGE(BADGE_CARRYING, slot, owner);
      badges++;
    }
    if (captureProgress) {
      QUEUE_BADGE(BADGE_CAPTURE, slot, owner);
      badges++;
    }
    if (flags & MAP_UNIT_DIVED) {
      QUEUE_BADGE(BADGE_DIVED, slot, owner);
      badges++;
    }
  }
  tile = slot * 4;
  TransferQueue_Add(
      UnitIcons_GetTiles() + Player_GetUnitIconTile(owner, type) * 32,
      GFX->objVram + (tile + UNIT_TILE) * 32, 128);
  /* semi-transparent, palette 12 */
  Oam_PutSprite(x, y | 0x400,
                badges ? data_021663b4[owner - 1] : data_021663c8[owner - 1],
                tile | 0xc000);
}
