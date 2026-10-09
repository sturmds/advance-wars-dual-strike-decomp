#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_01ff8d60(int bg);
void func_01ff9b48(void);
void func_0200215c(int co, void *dest, int palette);
void func_02002b00(int a);
void func_02002b24(int co, int a);
void func_02002e50(const void *src, void *dest, u32 size);
void func_02003188(const void *src, int offset, u32 size);
void func_02003bec(const void *src, void *dest);
void func_0200970c(int a);
void func_0200a810(const void *config);
void func_0200ac9c(void *state);
void func_0200acac(void *state);
void func_0200bb18(void *dest, const char *path, int a, int b);
u8 *File_Load(const char *path);
void Mem_Free(void *res);
void func_0201e30c(void *a, int b, int c);
void func_02021380(int a);
void func_02023e00(int a, u8 *objVram, int b);
void func_02023ea8(int a, int b);
void func_02026394(void);
void func_02029540(void);
void func_0202a9ac(const void *text, int x, int y, int a, int b);
void func_0202aba8(int x, int y);
void func_0204d6dc(void);
void CampaignMap_FindTopCos(void);
BOOL func_020d5d2c(void);
int func_021039e4(void);
const char *File_LocalizePath(const char *path);
const void *Text_Get(u32 textId);
u8 Map_GetPlayerCount(u16 mapId);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern CampaignMapState gCampaignMapState;
extern MenuState gMenuState;
extern CampaignMission gCampaignMissions[];

extern u8 data_0215b548[];
extern u8 data_0215b598[];
extern char data_0215b990[];           /* "/ohashi/res_gmap" */
extern char data_0215b9a4[];           /* "/ohashi/res_gmap_lang" */
extern char data_0215b9bc[];           /* "/ohashi/res_icon_blink" */
extern char data_0215b9d4[];           /* "/ohashi/res_gmap_map1" */
extern char data_0215b9ec[];           /* "/ohashi/res_gmap_map2" */

/* Needed to match: the original adds these field offsets to the address
 * of gMenuState at run time instead of folding them into the
 * relocation. */
#define WORK_FIELD(field) ((u8 *)&gMenuState + (u32)&((MenuState *)0)->campaignMap.field)

/* Sets up both screens of the campaign world map (the res_gmap files):
 * layer priorities, blending and scroll, the background graphics, the
 * three top COs and the enemy COs of the selected mission. Text
 * 0x7d000014 or 0x7d000015 goes on top depending on func_021039e4
 * (possibly hard campaign). */
void CampaignMap_Init(Unk_0204f960 *task)
{
    u8 *res;
    u8 *langRes;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int i;
    int objTile;
    int objTile2;
    int tile;
    int j;
    int playerCount;

    if (task->unk_52 == 2 && func_020d5d2c()) {
        func_0204d6dc();
    }
    CampaignMap_FindTopCos();
    /* COs of the map's last player slot (probably the enemy). The (u8) casts get within
     * one instruction of the original, which copies the count with
     * "add rN, r0, #0" instead of "and rN, r0, #0xff". */
    playerCount = Map_GetPlayerCount(gCampaignMissions[gCampaignMapState.mission].mapId);
    gMenuState.campaignMap.enemyCos[0] = Ov000_gMapHeaders[gCampaignMissions[gCampaignMapState.mission].mapId].co[(u8)playerCount][0];
    playerCount = Map_GetPlayerCount(gCampaignMissions[gCampaignMapState.mission].mapId);
    gMenuState.campaignMap.enemyCos[1] = Ov000_gMapHeaders[gCampaignMissions[gCampaignMapState.mission].mapId].co[(u8)playerCount][1];

    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_00);
    saved027c = data_027c027c;
    func_0200ac9c(data_027c0288.unk_00);
    func_0200970c(0);
    func_0200a810(data_0215b548);
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendMode = 1;
    data_027c0280->blendEva = 5;
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 1;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    func_0200ac9c(saved027c);
    func_0200acac(saved0280);

    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_04);
    saved027c = data_027c027c;
    func_0200ac9c(data_027c0288.unk_04);
    func_0200970c(0);
    func_0200a810(data_0215b598);
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    func_0200ac9c(saved027c);
    func_0200acac(saved0280);

    res = File_Load(data_0215b990);
    langRes = File_Load(File_LocalizePath(data_0215b9a4));

    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_00);
    func_0201e30c(data_027c0280->unk_f8, 0, 0);
    func_02021380(0);
    func_02003bec(res + 0x1ab0,
                  data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14) + 0x4000);
    func_02003bec(res + 0x36d4, data_027c0280->layer1Map);
    func_02003188(res + 0x3ae8, 0x40, 0x20);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer1Map[i] += 0x2200;
    }
    func_02002e50(data_027c0280->layer1Map, data_027c0280->layer1Map + 0x400, 0x800);
    func_01ff8d60(1);
    func_02003188(res + 0x3b08, 0x60, 0x140);
    func_02003bec(res + 0xed8, data_027c0280->objVram);
    func_02003188(res + 0x1884, 0x200, 0x40);
    func_02003bec(res + 0x848, data_027c0280->objVram + 0x2080);
    func_02003bec(res + 0xdcc, data_027c0280->objVram + 0x2f80);
    func_02003bec(res, data_027c0280->objVram + 0x3140);
    func_02003188(res + 0x834, 0x240, 0x20);
    func_0200acac(saved0280);

    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_04);
    func_0201e30c(data_027c0280->unk_f8, 0, 0);
    func_02021380(0);
    func_02003bec(langRes,
                  data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
    func_02003bec(res + 0x18c4, data_027c0280->layer2Map);
    func_02003188(res + 0x1a70, 0x20, 0x40);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer2Map[i] += 0x1000;
    }
    func_01ff8d60(2);
    func_02003bec(res + 0x1734, data_027c0280->objVram);
    func_02003188(res + 0x1884, 0x200, 0x20);
    for (i = 0, objTile = 38; i < 3; i++, objTile += 8) {
        func_02023e00(i + 0xbc, data_027c0280->objVram, objTile);
    }
    func_02023ea8(4, 18);
    for (i = 0, objTile2 = 158; i < 4; i++, objTile2 += 4) {
        func_02023e00(i + 0x55, data_027c0280->objVram, objTile2);
    }
    func_02023e00(0xc6, data_027c0280->objVram, 174);
    func_02023ea8(1, 19);
    for (i = 0, tile = 62, j = 98; i < 3; i++, tile += 12, j += 12) {
        if (gMenuState.campaignMap.topCos[i].co != 0) {
            func_0200215c(gMenuState.campaignMap.topCos[i].co, data_027c0280->objVram + ((u32)tile << 5), i + 20);
            func_02002b24(gMenuState.campaignMap.topCos[i].co, j);
        }
    }
    func_02002b00(1);
    for (j = 0, tile = 134; j < 2; j++, tile += 12) {
        if (gMenuState.campaignMap.enemyCos[j] != 0) {
            func_0200215c(gMenuState.campaignMap.enemyCos[j], data_027c0280->objVram + ((u32)tile << 5), j + 23);
        }
    }
    func_0200acac(saved0280);

    func_02002e50(res + 0xe4c, WORK_FIELD(unk_244), sizeof(gMenuState.campaignMap.unk_244));
    func_02002e50(res + 0xd08, WORK_FIELD(unk_020), sizeof(gMenuState.campaignMap.unk_020));
    func_02002e50(res + 0x6d4, WORK_FIELD(unk_0e4), sizeof(gMenuState.campaignMap.unk_0e4));
    Mem_Free(res);
    Mem_Free(langRes);
    func_0200bb18(gMenuState.colours, data_0215b9bc, 0, 0x20);

    res = File_Load(data_0215b9d4);
    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_00);
    func_02003bec(res, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
    func_02003bec(res + 0x53bc, data_027c0280->layer2Map);
    for (i = 0; i < 0x400; i++) {
        data_027c0280->layer2Map[i] += 0x3000;
    }
    func_01ff8d60(2);
    func_0200acac(saved0280);
    Mem_Free(res);

    res = File_Load(data_0215b9ec);
    saved0280 = data_027c0280;
    func_0200acac(data_027c0288.unk_00);
    func_02003bec(res, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
    func_02003bec(res + 0x4c7c, data_027c0280->layer3Map + 0x400);
    for (i = 0; i < 0x400; i++) {
        data_027c0280->layer3Map[i + 0x400] += 0x3000;
    }
    func_01ff8d60(3);
    func_0200acac(saved0280);
    Mem_Free(res);

    func_0202aba8(178, 9);
    if (func_021039e4() == 0) {
        func_0202a9ac(Text_Get(0x7d000014), 178, 9, 0, 0);
    } else {
        func_0202a9ac(Text_Get(0x7d000015), 178, 9, 0, 0);
    }
    func_02029540();
    func_01ff9b48();
    func_02026394();
}
