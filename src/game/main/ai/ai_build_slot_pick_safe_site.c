#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

extern AiBuildSlot data_0229e44c[];

void func_020937dc(void);
int func_02096610(int arg, int slotIndex);
int func_020966bc(int arg, int slotIndex);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define SLOT_USED_UP      0xff
#define SAFE_THREAT_LIMIT 50

/* Takes the first slot with unk_03_1 set that is not used up, and asks
 * func_02096610 and then func_020966bc for a site for it. Returns the site
 * when both enemy threat values of its tile are below SAFE_THREAT_LIMIT;
 * otherwise marks the slot used up and searches again. The slot pointer is
 * not reset between searches, so a new search starts at the slot just marked
 * while i starts at 0 again. Returns -1 when no slot is left. */
int AiBuildSlot_PickSafeSite(void)
{
    AiBuildSlot *slot = data_0229e44c;
    int i;
    int found;
    int site;
    int tile;

    func_020937dc();
    for (;;) {
        found = -1;
        for (i = 0; i < data_021619c0->slotCount; i++, slot++) {
            if (slot->unk_01 != SLOT_USED_UP && slot->unk_03_1) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return -1;
        }
        site = func_02096610(1, found);
        if (site != -1) {
            site = func_020966bc(1, found);
        }
        if (site != -1) {
            tile = TILE(data_0229e5bc[site].x, data_0229e5bc[site].y);
            if (STATE->mapTiles->threatMax[tile] < SAFE_THREAT_LIMIT && STATE->mapTiles->threatSum[tile] < SAFE_THREAT_LIMIT) {
                return site;
            }
        }
        slot->unk_01 = SLOT_USED_UP;
    }
}
