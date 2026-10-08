#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_0200ac9c(void *state);
void func_0200acac(void *state);
int func_020c3da4(int mapId);          /* MapHeader.playerCount for most map ids */
void SetMapWeather(int weather);       /* sets unk_1c4 weather and default weather */
void func_020e8c3c(void);
int func_021039e4(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MapHeader Ov000_gMapHeaders[];

/* Shorthands for the current state and its map id; the original code
 * reloads both through the globals on every access. */
#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP_ID (STATE->unk_1c4->mapId)

/* Applies the current map's header to the rules: fog, weather, tileset,
 * the tag partner flags and the per-player bytes unk_92, each by game
 * mode (unk_58). data_027c0288 holds two states; modes 0 and 1 switch to
 * one of them to write there, then switch back. */
void SetupMapRules(void)
{
    func_020e8c3c();
    switch (STATE->unk_58) {
    case 0: {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;
        int i;

        func_0200acac(data_027c0288.unk_00);
        saved027c = data_027c027c;
        func_0200ac9c(data_027c0288.unk_00);
        /* 2 for the slot that equals the map's player count, else 1 */
        for (i = 1; i < 5; i++) {
            if (i == func_020c3da4(MAP_ID)) {
                STATE->unk_92[i] = 2;
            } else {
                STATE->unk_92[i] = 1;
            }
        }
        if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
            STATE->unk_1c4->fog = 1;
        }
        STATE->unk_7c = 0;
        STATE->unk_80 = Ov000_gMapHeaders[MAP_ID].unk_30[func_021039e4()];
        STATE->unk_81 = Ov000_gMapHeaders[MAP_ID].unk_34[func_021039e4()];
        STATE->partnerBoostHuman = 1;
        STATE->partnerBoostAi = 1;
        func_0200ac9c(saved027c);
        func_0200acac(saved0280);
        break;
    }
    case 1:
        STATE->unk_92[1] = 1;
        STATE->unk_92[2] = 2;
        STATE->unk_92[3] = 2;
        STATE->unk_92[4] = 2;
        if (Ov000_gMapHeaders[MAP_ID].unk_10 != 0) {
            Unk_027c0280 *saved0280;
            Unk_02080440 *saved027c;

            if (STATE->unk_59 <= 1) {
                saved0280 = data_027c0280;
                func_0200acac(data_027c0288.unk_04);
                saved027c = data_027c027c;
                func_0200ac9c(data_027c0288.unk_04);
                STATE->unk_92[1] = 2;
                func_0200ac9c(saved027c);
                func_0200acac(saved0280);
            } else {
                saved0280 = data_027c0280;
                func_0200acac(data_027c0288.unk_04);
                saved027c = data_027c027c;
                func_0200ac9c(data_027c0288.unk_04);
                STATE->unk_92[1] = 1;
                func_0200ac9c(saved027c);
                func_0200acac(saved0280);
            }
            saved0280 = data_027c0280;
            func_0200acac(data_027c0288.unk_04);
            saved027c = data_027c027c;
            func_0200ac9c(data_027c0288.unk_04);
            STATE->unk_92[2] = 2;
            STATE->unk_92[3] = 2;
            STATE->unk_92[4] = 2;
            func_0200ac9c(saved027c);
            func_0200acac(saved0280);
        }
        switch (STATE->unk_59) {
        case 0:
            STATE->partnerBoostHuman = 0;
            STATE->partnerBoostAi = 0;
            break;
        case 1:
            STATE->partnerBoostHuman = 1;
            STATE->partnerBoostAi = 1;
            break;
        case 2:
            STATE->partnerBoostHuman = 0;
            STATE->partnerBoostAi = 0;
            break;
        case 3:
            STATE->partnerBoostHuman = 1;
            STATE->partnerBoostAi = 1;
            break;
        }
        break;
    case 3:
        STATE->unk_92[1] = 1;
        STATE->unk_92[2] = 2;
        STATE->unk_92[3] = 2;
        STATE->unk_92[4] = 2;
        if (Ov000_gMapHeaders[MAP_ID].fog != 0) {
            STATE->unk_1c4->fog = 1;
        }
        STATE->partnerBoostHuman = 1;
        STATE->partnerBoostAi = 1;
        break;
    }
    SetMapWeather(Ov000_gMapHeaders[MAP_ID].unk_1b);
    STATE->unk_1c4->tileset = Ov000_gMapHeaders[MAP_ID].unk_1a;
}
