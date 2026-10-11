#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 gUnitIconTileBase[];        /* palette and first tile: [0] grey (waited), then players 1-4 */
extern s16 gUnitIconHpDigit[];         /* per shown HP 0-10: negative when no digit is drawn (0 and 10) */

int Player_GetCoIconTile(int player, int type);
BOOL UnitType_HidesCargo(int type, int viewer, int owner);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Tiles of the badge in the icon's bottom-left corner and the HP digit in
 * its bottom-right one, from gUnitIconTileBase. */
#define BADGE_NONE         0
#define BADGE_HP_DIGITS    136         /* + shown HP */
#define BADGE_HIDDEN_HP    146
#define BADGE_LOW_FUEL     147         /* alone; with another badge, that badge + 1 */
#define BADGE_LOW_AMMO     148         /* alone; with another badge, that badge + 2 */
#define BADGE_CARGO        150
#define BADGE_CAPTURE      154
#define BADGE_DIVED        158
#define BADGE_HIDDEN_CARGO 162
#define BADGE_PARALYSED    166

#define ENTRY_HFLIP 0x400              /* BG map entry: flip the tile */

/* Writes the 2 x 2 BG map entries of a unit icon at entry (rows of 32). look
 * is the unit slot's owner bits (slot & 0xc0), or 0x100 for a unit that has
 * waited: grey, owned by the current player. Players 1 and 3 face the other
 * way, so their tiles are flipped. */
void UnitIcon_DrawEntries(u16 *entry, int type, int look, int carrying, u8 capture, u16 shownHp, u16 dived,
                          u8 hpHidden, u8 lowFuel, u8 lowAmmo, u8 paralysed)
{
    u16 base;
    u16 tile;
    int player;
    u16 badge;

    if (look == 0x100) {
        base = gUnitIconTileBase[0];
        look = STATE->currentUnitBase;
    } else {
        base = gUnitIconTileBase[(look >> 6) + 1];
    }
    player = (look >> 6) + 1;
    if (carrying != 0) {
        carrying = TRUE;
    }
    tile = Player_GetCoIconTile(player, type);
    if (UnitType_HidesCargo(type, STATE->currentPlayer, (u16)player)) {
        badge = BADGE_HIDDEN_CARGO;
    } else if (carrying) {
        badge = BADGE_CARGO;
    } else if (capture) {
        badge = BADGE_CAPTURE;
    } else if (dived) {
        badge = BADGE_DIVED;
    } else {
        badge = BADGE_NONE;
    }
    if (lowFuel) {
        if (badge != BADGE_NONE) {
            badge += 1;
        } else {
            badge = BADGE_LOW_FUEL;
        }
    }
    if (lowAmmo) {
        if (badge != BADGE_NONE) {
            badge += 2;
        } else {
            badge = BADGE_LOW_AMMO;
        }
    }
    if (paralysed) {
        badge = BADGE_PARALYSED;
    }
    if (look == 0 || look == 128) {
        entry[0] = tile + (ENTRY_HFLIP + 1) + base;
        entry[1] = tile + base + ENTRY_HFLIP;
        if (badge != BADGE_NONE) {
            entry[32] = badge + base;
        } else {
            entry[32] = tile + (ENTRY_HFLIP + 3) + base;
        }
        if ((STATE->players[player].visionGrids & 2) == 0 && hpHidden) {
            entry[33] = base + BADGE_HIDDEN_HP;
            return;
        }
        if (gUnitIconHpDigit[shownHp] < 0) {
            entry[33] = tile + (ENTRY_HFLIP + 2) + base;
        } else {
            entry[33] = shownHp + BADGE_HP_DIGITS + base;
        }
    } else {
        entry[0] = tile + base;
        entry[1] = tile + 1 + base;
        if (badge != BADGE_NONE) {
            entry[32] = badge + base;
        } else {
            entry[32] = tile + 2 + base;
        }
        if ((STATE->players[player].visionGrids & 2) == 0 && hpHidden) {
            entry[33] = base + BADGE_HIDDEN_HP;
            return;
        }
        if (gUnitIconHpDigit[shownHp] < 0) {
            entry[33] = tile + 3 + base;
        } else {
            entry[33] = shownHp + BADGE_HP_DIGITS + base;
        }
    }
}
