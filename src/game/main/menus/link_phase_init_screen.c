#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02066818.h"
#include "game/map_header.h"
#include "game/design_map.h"

void func_01ff8d60(int a);
void func_02002e50(const void *src, void *dst, u32 size);
void func_02003188(const void *src, u32 offset, u32 size);
void func_02003bec(const void *src, void *dst);
void func_0200ac9c(void *state);
void func_0200acac(void *state);
void *func_0200bc28(const char *path);
void func_0200bf24(void *file);
void func_02015340(int x, int y, void *target, const void *text, int a, int b);
void func_0201e30c(void *target, int a, int b);
void func_0201e6b4(int x, int y, int w, int h);
void func_02021380(int a);
void func_02025bb4(void);
void func_020262b0(int a);
void func_020262f0(int a, int b, int c, int d);
void func_02026394(void);
void func_020269b4(void *target, int a, int b, const void *src, int c);
void func_02026bc8(void *dst, int a, int b, int c);
void func_02028ae8(void);
void func_02029648(void);
void func_0202a9ac(const void *text, int a, int b, int c, int d);
void func_0202aba8(int a, int b);
void func_0202bdec(void *dst, int mapId);
void func_0202c520(int a);
void func_020727d0(void);
int func_02073768(int player);
int func_0207bd80(void);
void func_0207d918(void);
void func_02082614(int a, int b, int c, int d);
const char *func_021040dc(const char *path);
u8 Map_GetPlayerCount(u32 mapId);
const void *Map_GetName(u32 mapId);
const void *Text_Get(u32 textId);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_02066818 data_0229eb30;
extern u8 data_0229c874[4];
extern DesignMap data_022a6c84[];
extern u16 gLinkPhaseArmyGfxOffsets[];
extern const char gLinkPhaseResPath[];
extern const char gLinkCommonLangResPath[];
extern const char gLinkCommonResPath[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* data_027c0288 holds two game states; func_0200acac and func_0200ac9c load
 * one into data_027c0280 and data_027c027c. */
#define MAIN_MAP   (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)            \
    saved0280 = data_027c0280;      \
    func_0200acac(state);           \
    saved027c = data_027c027c;      \
    func_0200ac9c(state)

#define SWITCH_BACK()               \
    func_0200ac9c(saved027c);       \
    func_0200acac(saved0280)

#define LINK_STATE ((Unk_02066818 *)&data_0229eb30)

/* Message ids; which one shows depends on GAME->unk_8a. */
#define TEXT_LINK_ONE_MAP  0x7d000017
#define TEXT_LINK_TWO_MAPS 0x7d000018

/* Sets up the screen of the wireless link phase (graphics from
 * /ohashi/res_linkphase and /commo/res_linkcommon). Copies each player's
 * army colour from the current map into data_0229eb30, draws one army icon
 * per player, and writes the map name on the second game state's screen.
 * When GAME->unk_8a is 1 it writes the second map's name there, and the
 * main state's screen also gets the first map's name. */
void LinkPhase_InitScreen(void)
{
    int i;
    u8 *file;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int mapId;                         /* Map_GetName gets it as (u16), needed to match */
    BOOL customMap;
    void *target;                      /* needed to match: read before the call in the same argument list */

    data_0229eb30.unk_02c = Map_GetPlayerCount(GAME->unk_1c4->mapId);
    /* Needed to match: the flag is kept in a register, and the stores go
     * through a cast pointer so the compiler reloads data_027c027c and
     * data_027c0284 on every pass. */
    customMap = MAP_IS_CUSTOM(GAME->unk_1c4->mapId);
    if (customMap) {
        for (i = 0; i < data_0229eb30.unk_02c; i++) {
            LINK_STATE->unk_024[i] = data_022a6c84[GAME->unk_1c4->mapId - MAP_CUSTOM_FIRST].armyColours[i + 1];
        }
    } else {
        for (i = 0; i < data_0229eb30.unk_02c; i++) {
            LINK_STATE->unk_024[i] = Ov000_gMapHeaders[GAME->unk_1c4->mapId].army[i + 1];
        }
    }
    for (i = 0; i < 4; i++) {
        data_0229eb30.unk_020[i] = data_0229c874[i] - 1;
        data_0229eb30.unk_028[i] = 0;
    }
    func_02028ae8();

    SWITCH_TO(MAIN_MAP);
    data_027c0280->unk_94_2 = 1;
    data_027c0280->unk_bc = 10;
    data_027c0280->unk_be = 6;
    data_027c0280->unk_c0 = 0;
    data_027c0280->unk_90[data_027c0280->unk_e1] = 0;
    data_027c0280->unk_90[data_027c0280->unk_f5] = 0;
    data_027c0280->unk_90[data_027c0280->unk_109] = 1;
    data_027c0280->unk_90[data_027c0280->unk_11d] = 0;
    data_027c0280->unk_94_0 = 0;
    data_027c0280->unk_96[data_027c0280->unk_e1] = 0;
    data_027c0280->unk_96[data_027c0280->unk_f5] = 0;
    data_027c0280->unk_96[data_027c0280->unk_109] = 0;
    data_027c0280->unk_96[data_027c0280->unk_11d] = 0;
    data_027c0280->unk_9a_0 = 1;
    SWITCH_BACK();

    file = func_0200bc28(gLinkPhaseResPath);
    SWITCH_TO(MAIN_MAP);
    func_02003bec(file, data_027c0280->unk_130 + (data_027c0280->bgControl[data_027c0280->unk_109].charBase << 14));
    func_02003188(file + 0x2a4, 0, 0xa0);
    for (i = 0; i < data_0229eb30.unk_02c; i++) {
        target = data_027c0280->unk_114;
        func_020269b4(target, 4, func_02073768(i), file + gLinkPhaseArmyGfxOffsets[data_0229eb30.unk_024[i] - 1], 0);
    }
    func_01ff8d60(2);
    SWITCH_BACK();
    func_0200bf24(file);

    file = func_0200bc28(func_021040dc(gLinkCommonLangResPath));
    func_02003bec(file, data_027c0280->charBase);
    func_0200bf24(file);

    file = func_0200bc28(gLinkCommonResPath);
    func_02003188(file, 0x200, 0x20);
    func_02003188(file, 0x220, 0x20);
    func_02003188(file, 0x240, 0x20);
    func_02026bc8(data_027c0280->unk_08 + 0x240, 0, 8, 8);
    func_02002e50(file + 0x20, &data_0229eb30, 0x20);
    func_0200bf24(file);

    if (GAME->unk_8a == 1) {
        mapId = GAME->secondMapId;
    } else {
        mapId = GAME->unk_1c4->mapId;
    }
    SWITCH_TO(SECOND_MAP);
    func_0201e30c(data_027c0280->unk_f8, 0, 0);
    func_02021380(0);
    func_0201e6b4(11, 20, 10, 4);
    target = data_027c0280->unk_ec;
    func_02015340(12, 21, target, Map_GetName((u16)mapId), 0, 0x100);
    func_0202bdec(data_027c0280->charBase + 0x4000, mapId);
    func_0202c520(18);
    func_01ff8d60(0);
    func_01ff8d60(1);
    SWITCH_BACK();

    if (GAME->unk_8a == 1) {
        mapId = GAME->unk_1c4->mapId;
        SWITCH_TO(MAIN_MAP);
        data_027c0280->bgControl[data_027c0280->unk_e1].priority = 1;
        data_027c0280->bgControl[data_027c0280->unk_f5].priority = 2;
        data_027c0280->bgControl[data_027c0280->unk_109].priority = 0;
        data_027c0280->bgControl[data_027c0280->unk_11d].priority = 3;
        func_0201e30c(data_027c0280->unk_f8, 0, 0);
        func_02021380(0);
        func_0201e6b4(11, 20, 10, 4);
        target = data_027c0280->unk_ec;
        func_02015340(12, 21, target, Map_GetName((u16)mapId), 0, 0x100);
        func_0202bdec(data_027c0280->charBase + 0x780, mapId);
        func_0202c520(19);
        func_01ff8d60(0);
        func_01ff8d60(1);
        SWITCH_BACK();
    }

    func_0202aba8(0, 0);
    if (GAME->unk_8a == 0) {
        func_0202a9ac(Text_Get(TEXT_LINK_ONE_MAP), 0, 0, 1, 0);
    } else {
        func_0202a9ac(Text_Get(TEXT_LINK_TWO_MAPS), 0, 0, 1, 0);
    }
    func_02026394();
    func_020262f0(0, -16, 0x100, -16);
    func_020262b0(0);
    func_020727d0();
    func_02025bb4();
    func_02029648();
    func_0207d918();
    if (func_0207bd80() == 1) {
        func_02082614(0xe8, 8, 0x1f0, 15);
    }
}
