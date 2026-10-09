#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/campaign_recap.h"

void func_020c4eb4(void);
void Map_LoadVariant(int mapId, int variant);
void func_020c4fa0(void);
void func_020c65fc(s8 *grid, int value);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Loads campaign mission `mission` (map id MAP_CAMPAIGN_FIRST + mission)
 * into the current state for the campaign recap, stores the map size in
 * recap->unk_44 and unk_48, and
 * starts the screen black. Each slot 1-4 first gets army style = slot
 * number; a slot whose header CO is a real CO then gets that CO's style.
 * Slots with a CO or CO_PLAYER_PICKS get a human controller, slots with
 * none get no controller. Called by CampaignRecap_Update.
 *
 * WIP: 97%. The loop preheader schedules two instructions differently
 * (the Ov000_gMapHeaders load and the copies of the constant 1). */
void func_020fed58(CampaignRecap *recap, int mission)
{
    int mapId;
    int slot;
    int coId;

    mapId = MAP_CAMPAIGN_FIRST + mission;
    STATE->unk_1c4->mapId = mapId;
    STATE->unk_1c4->tileset = Ov000_gMapHeaders[STATE->unk_1c4->mapId].tileset;
    func_020c4eb4();
    Map_LoadVariant(mapId, 0);
    func_020c4fa0();
    func_020c65fc(STATE->mapTiles->inSight + STATE->unk_20 * 0x508, 1);
    func_020c65fc(STATE->mapTiles->inSight + STATE->unk_22 * 0x508, 1);
    recap->unk_44 = STATE->unk_1c4->mapWidth;
    recap->unk_48 = STATE->unk_1c4->mapHeight;
    STATE->players[1].armyStyle = 1;
    STATE->players[2].armyStyle = 2;
    STATE->players[3].armyStyle = 3;
    STATE->players[4].armyStyle = 4;
    for (slot = 1; slot <= 4; slot++) {
        coId = (Ov000_gMapHeaders + MAP_CAMPAIGN_FIRST)[mission].co[slot][0] & PLAYER_CO_ID_MASK;
        if (coId != 0) {
            if (coId != CO_PLAYER_PICKS) {
                STATE->players[slot].armyStyle = gCoTable[coId].armyStyle + 1;
                STATE->players[slot].controller = PLAYER_CONTROLLER_HUMAN;
            } else {
                STATE->players[slot].controller = PLAYER_CONTROLLER_HUMAN;
            }
        } else {
            STATE->players[slot].controller = PLAYER_CONTROLLER_NONE;
        }
    }
    if (HOOKS->unk_54 != NULL) {
        HOOKS->unk_54(1);
    }
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->masterBrightness = -16;
}
