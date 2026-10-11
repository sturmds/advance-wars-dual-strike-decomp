#include "types.h"
#include "game/battle_scene.h"
#include "game/battle_gfx.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"
#include "game/unit_stats.h"

extern MenuState gMenuState;

u32 File_UnpackTo(void *dest, const char *path);
int FS_ChangeDir(const char *path);
void func_0210cb04(void);              /* guess: maps VRAM bank A to the CPU (LCDC) */
void GX_EndLoadTex(void);              /* guess: undoes func_0210cb04 */
void GX_BeginLoadTexPltt(void);
void GX_EndLoadTexPltt(void);
void File_ReadTo(void *dest, const char *path);
void DC_FlushRange(void *start, u32 size);
void GX_LoadTexPltt(const void *src, u32 offset, u32 size);

extern char data_ov002_023600e4[];
extern const BattleGfxFile *data_ov002_02369830;
extern const u32 data_ov002_02360b90[];
extern char data_ov002_0236049c[];
extern char data_ov002_023604a0[];
extern char data_ov002_023604a4[];
extern char data_ov002_023604a8[];

#define BATTLE (gMenuState.battle)
#define SCENE Ov002_gBattleScene

/* VRAM bank A while it is mapped to the CPU, in 32-byte tiles. */
#define VRAM_LCDC_A 0x06800000
#define TILE_ADDR(tile) ((void *)(VRAM_LCDC_A + (tile) * 32))

/* Unpacks a file to the tile and gives the tiles it takes. Unpacks it twice, as the
 * original does. */
#define LOAD_TILES(path) \
    (File_UnpackTo(TILE_ADDR(tile), path) & 31 ? (File_UnpackTo(TILE_ADDR(tile), path) >> 5) + 1 \
                                                : File_UnpackTo(TILE_ADDR(tile), path) >> 5)

/* First tile of graphics kind k of the side. In the battle screen the first 0x24 bytes
 * of MenuState hold these, two per kind. */
#define TILE_START(k) (((u16(*)[2])gMenuState.colours)[k][side])

/* A unit type past UNIT_OOZIUM; it gets the same extra file as UNIT_PIPERUNNER. */
#define UNIT_TYPE_30 30

/* This function reads BattleSideData.effects[0-8] as graphics records: 0-1 by weapon,
 * 2-3 and 4-5 by the other side's weapon, then 6, 7 and 8. Needed to match: entries 2-5
 * are indexed from &effects[2] and &effects[4]. */
#define GFX_FILE(effect) ((const BattleGfxFile *)(effect))
#define BY_STYLE(effect) ((const BattleGfxByStyle *)(effect))

/* Needed to match: the empty-name test loads the first character signed. */
#define IS_EMPTY(path) ((s8)(path)[0] == '\0')

/* Loads the sprite graphics of both sides of the battle screen from /battle into VRAM bank A,
 * from tile 0x300 on, and keeps the first tile and the sprite table of each in MenuState.
 * Most files come in one version per army style. Then loads each side's palette by army,
 * with a copy brightened halfway to white, and copies both to palette memory.
 * Partial match (99.9%): in the palette loop, menu and the side * 12 offset swap registers.
 * The palette loop has its own counter (palSide) because that fixes the other registers. */
void func_ov002_02352754(void)
{
    u8 weapon;
    int tile;
    int side;
    int other;
    int i;
    BOOL isFirst;
    u8 otherWeapon;
    const u16 *palette;
    MenuState *menu;                   /* needed to match: the palette offsets are added at run time */
    int palSide;

    tile = 0x300;
    FS_ChangeDir(data_ov002_023600e4);
    func_0210cb04();
    for (side = 0; side < 2; side++) {
        const BattleGfxByStyle *byStyle;
        const BattleGfxFile *file;
        const BattleGfxFile *main;
        const char *path;

        other = (side + 1) & 1;
        weapon = SCENE->sides[side].usesSecondary;
        otherWeapon = SCENE->sides[other].usesSecondary;

        TILE_START(0) = tile;
        byStyle = &BATTLE.unk_27c[side]->weapons[weapon]->gfx;
        if (byStyle != NULL) {
            path = byStyle->files[SCENE->sides[side].armyStyle - 1];
            if (IS_EMPTY(path)) {
                BATTLE.spriteTables[side] = NULL;
            } else {
                tile += LOAD_TILES(path);
                BATTLE.spriteTables[side] = BATTLE.unk_27c[side]->weapons[weapon]->gfx.tables[SCENE->sides[side].armyStyle - 1];
            }
        } else {
            BATTLE.spriteTables[side] = NULL;
        }

        TILE_START(1) = tile;
        byStyle = &BATTLE.unk_27c[side]->unk_08[weapon]->gfx;
        if (byStyle != NULL) {
            path = byStyle->files[SCENE->sides[side].armyStyle - 1];
            if (IS_EMPTY(path)) {
                BATTLE.spriteTables2[side] = NULL;
            } else {
                tile += LOAD_TILES(path);
                BATTLE.spriteTables2[side] = BATTLE.unk_27c[side]->unk_08[weapon]->gfx.tables[SCENE->sides[side].armyStyle - 1];
            }
        } else {
            BATTLE.spriteTables2[side] = NULL;
        }

        TILE_START(2) = tile;
        byStyle = &BATTLE.unk_27c[side]->unk_10->gfx;
        if (byStyle != NULL) {
            path = byStyle->files[SCENE->sides[side].armyStyle - 1];
            if (IS_EMPTY(path)) {
                BATTLE.spriteTables3[side] = NULL;
            } else {
                tile += LOAD_TILES(path);
                BATTLE.spriteTables3[side] = BATTLE.unk_27c[side]->unk_10->gfx.tables[SCENE->sides[side].armyStyle - 1];
            }
        } else {
            BATTLE.spriteTables3[side] = NULL;
        }

        TILE_START(3) = tile;
        file = BATTLE.unk_27c[side]->unk_1c[weapon];
        if (file != NULL) {
            tile += LOAD_TILES(file->file);
            BATTLE.unk_40[side] = (void *)BATTLE.unk_27c[side]->unk_1c[weapon]->table;
        } else {
            BATTLE.unk_40[side] = NULL;
        }

        TILE_START(4) = tile;
        byStyle = BY_STYLE(BATTLE.unk_27c[side]->effects[weapon]);
        if (byStyle != NULL) {
            tile += LOAD_TILES(byStyle->files[0]);
            BATTLE.unk_48[side] = (u32)BY_STYLE(BATTLE.unk_27c[side]->effects[weapon])->tables[0];
        } else {
            BATTLE.unk_48[side] = 0;
        }

        TILE_START(5) = tile;
        file = GFX_FILE((&BATTLE.unk_27c[other]->effects[2])[otherWeapon]);
        if (file != NULL) {
            tile += LOAD_TILES(file->file);
            BATTLE.hitSpriteTables[side] = GFX_FILE((&BATTLE.unk_27c[other]->effects[2])[otherWeapon])->table;
        } else {
            BATTLE.hitSpriteTables[side] = NULL;
        }

        TILE_START(6) = tile;
        main = GFX_FILE(BATTLE.unk_27c[side]->effects[6]);
        if (main != NULL && main != data_ov002_02369830) {
            tile += LOAD_TILES(main->file);
            BATTLE.unk_58[side] = GFX_FILE(BATTLE.unk_27c[side]->effects[6])->table;
        } else {
            file = GFX_FILE((&BATTLE.unk_27c[other]->effects[4])[otherWeapon]);
            if (file != NULL) {
                tile += LOAD_TILES(file->file);
                BATTLE.unk_58[side] = GFX_FILE((&BATTLE.unk_27c[other]->effects[4])[otherWeapon])->table;
            } else if (main != NULL) {
                tile += LOAD_TILES(main->file);
                BATTLE.unk_58[side] = GFX_FILE(BATTLE.unk_27c[side]->effects[6])->table;
            } else {
                BATTLE.unk_58[side] = NULL;
            }
        }

        TILE_START(7) = tile;
        file = GFX_FILE(BATTLE.unk_27c[side]->effects[7]);
        if (file != NULL) {
            tile += LOAD_TILES(file->file);
            BATTLE.unk_60[side] = GFX_FILE(BATTLE.unk_27c[side]->effects[7])->table;
        } else {
            BATTLE.unk_60[side] = NULL;
        }

        BATTLE.unk_20[side] = tile;
        /* BATTLE_SIDE_UNIT_TYPE(side), written from the side's record to match. */
        if ((&SCENE->sides[side].hpBefore)[-1] == UNIT_ARTILLERY && side != SCENE->attackerSide) {
            BATTLE.unk_68[side] = NULL;
        } else {
            byStyle = BY_STYLE(BATTLE.unk_27c[side]->effects[8]);
            if (byStyle != NULL) {
                path = byStyle->files[SCENE->sides[side].armyStyle - 1];
            if (IS_EMPTY(path)) {
                    BATTLE.unk_68[side] = NULL;
                } else {
                    tile += LOAD_TILES(path);
                    BATTLE.unk_68[side] = BY_STYLE(BATTLE.unk_27c[side]->effects[8])->tables[0];
                }
            } else {
                BATTLE.unk_68[side] = NULL;
            }
        }
    }

    BATTLE.unk_24 = tile;
    BATTLE.unk_70 = data_ov002_02360b90;
    tile += LOAD_TILES(data_ov002_0236049c);
    BATTLE.pipeTexture = tile;
    if (BATTLE_SIDE_UNIT_TYPE(0) == UNIT_PIPERUNNER || BATTLE_SIDE_UNIT_TYPE(1) == UNIT_PIPERUNNER
        || BATTLE_SIDE_UNIT_TYPE(0) == UNIT_TYPE_30 || BATTLE_SIDE_UNIT_TYPE(1) == UNIT_TYPE_30) {
        tile += LOAD_TILES(data_ov002_023604a0);
    }
    GX_EndLoadTex();
    GX_BeginLoadTexPltt();
    menu = &gMenuState;

    for (palSide = 0; palSide < 2; palSide++) {
        File_ReadTo(menu->battle.unk_450, menu->battle.unk_27c[palSide]->paletteFiles[SCENE->sides[palSide].army - 1]);
        /* Each colour halfway to white. */
        /* needed to match: the offset as a number keeps the add at run time */
        palette = (const u16 *)((u8 *)menu + (u32)&((MenuState *)0)->battle.unk_450);
        for (i = 0; i < 32; i++) {
            int colour = palette[i];

            menu->battle.unk_410[i] = ((((colour & 0x7c00) >> 10) + 31) >> 1) << 10
                | ((((colour & 0x3e0) >> 5) + 31) >> 1) << 5
                | (((colour & 31) + 31) >> 1);
        }
        DC_FlushRange(menu->battle.unk_410, sizeof(menu->battle.unk_410) + sizeof(menu->battle.unk_450));
        isFirst = palSide == 0 ? TRUE : FALSE;
        GX_LoadTexPltt(menu->battle.unk_450, (isFirst ? 0 : 2) * 32, 64);
        GX_LoadTexPltt(menu->battle.unk_410, (isFirst ? 4 : 6) * 32, 64);
    }
    File_ReadTo((void *)0x06890100, data_ov002_023604a4);
    File_ReadTo((void *)0x06890100, data_ov002_023604a4);
    if (BATTLE_SIDE_UNIT_TYPE(0) == UNIT_PIPERUNNER || BATTLE_SIDE_UNIT_TYPE(1) == UNIT_PIPERUNNER
        || BATTLE_SIDE_UNIT_TYPE(0) == UNIT_TYPE_30 || BATTLE_SIDE_UNIT_TYPE(1) == UNIT_TYPE_30) {
        File_ReadTo((void *)0x068901c0, data_ov002_023604a8);
    }
    GX_EndLoadTexPltt();
}
