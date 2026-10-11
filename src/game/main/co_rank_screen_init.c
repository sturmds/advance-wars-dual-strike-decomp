#include "types.h"
#include "game/co_rank_task.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7b80.h"

u32 Survival_GetExperienceBonus(void);
void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
void ModeFont_Load(int tile, int paletteSlot);
void ModeFont_ShowText(const void *text, int tile, int paletteSlot, int a, int b);
const void *Text_Get(u32 textId);
void Wallpaper_LoadBoth(void);
u8 *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void Mem_Free(void *file);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, int offset, u32 size);
void Gfx_LoadBmap09f(void *target, int a, int b);
void Font_LoadDigits(int a);
void Icon_LoadTo(int iconId, u8 *vram, int tile);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Co_LoadGraphicsAndPalette(int coId, int tile, int paletteSlot);
int Survival_GetRunGrade(void);
void func_01ff9b48(void);
void Menu_Reset(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_022a7b80 data_022a7b80;
extern u8 gMenuState[];
extern const char gCoRankPointObjPath[]; /* "/ohashi/res_result_point_obj" */
extern const char gCoRankSurvivalPath[]; /* "/ohashi/res_survival" */
extern const char gCoRankLangPath[]; /* "/ohashi/res_result_lang" */
extern const char gCoRankObjPath[]; /* "/ohashi/res_result_obj" */
extern const char gCoRankCtcLangPath[]; /* "/ohashi/res_result_ctc_lang" */

#define STATE (data_027c027c->unk_48[data_027c0284])

#define MAIN_MAP   (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)
#define MAIN_OBJ_VRAM (((Unk_027c0280 *)MAIN_MAP)->objVram)

/* GFX_TO opens a block that keeps the current graphics state; GFX_BACK
 * restores it and closes the block. */
#define GFX_TO(state)                                 \
    {                                                 \
        Unk_027c0280 *saved0280 = data_027c0280;      \
        Screen_SetCurrentGfx(state)

#define GFX_BACK()                                    \
        Screen_SetCurrentGfx(saved0280);              \
    }

#define EXPERIENCE_MAX_GAIN 9999

/* First step of the CO rank screen at the end of a Survival run. Stores
 * the mode's experience bonus and the experience gained (run points plus
 * bonus, at most 9999, as Survival_AwardCoExperience adds it), sets up
 * both screens, shows the title for the mode variant (time, days or
 * funds), and loads the result graphics: the grade picture picked by
 * Survival_GetRunGrade (5 best, 2 lowest), its icons (two runs of ten, likely
 * digits), and the pictures of player 1's front CO and, in a tag battle,
 * back CO. The res_result_ctc_lang file goes into gMenuState. */
void CoRankScreen_Init(CoRankTask *task)
{
    int i;
    u8 *file;

    task->experienceBonus = Survival_GetExperienceBonus();
    task->experienceGain = data_022a7b80.points;
    task->experienceGain += task->experienceBonus;
    if (task->experienceGain > EXPERIENCE_MAX_GAIN) {
        task->experienceGain = EXPERIENCE_MAX_GAIN;
    }

    Gfx_InitBothScreens();
    {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c = data_027c027c;
        Screen_SetCurrentBoth(MAIN_MAP);
        Screen_SetLayerScreenSize(0, 2);
        Screen_SetLayerScreenSize(1, 2);
        Gfx_ClearLayerMap(0);
        Gfx_ClearLayerMap(1);
        data_027c0280->dispCnt |= 0x40;
        data_027c0280->unk_dd = 1;
        Screen_SetCurrentBoth(SECOND_MAP);
        Screen_SetLayerScreenSize(0, 2);
        Screen_SetLayerScreenSize(1, 2);
        Gfx_ClearLayerMap(0);
        Gfx_ClearLayerMap(1);
        data_027c0280->dispCnt |= 0x40;
        data_027c0280->unk_dd = 1;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }

    ModeFont_Load(234, 5);
    switch (data_022a7b80.mode) {
    case 0:
    case 3:
        ModeFont_ShowText(Text_Get(0x7d00000c), 234, 5, 7, 0);
        break;
    case 2:
    case 5:
        ModeFont_ShowText(Text_Get(0x7d00000d), 234, 5, 7, 0);
        break;
    default:
        ModeFont_ShowText(Text_Get(0x7d00000e), 234, 5, 7, 0);
        break;
    }
    Wallpaper_LoadBoth();

    file = File_Load(gCoRankPointObjPath);
    GFX_TO(MAIN_MAP);
        Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
        Font_LoadDigits(0);
        Data_Unpack(file + 0xfc, data_027c0280->objVram + 0xc00);
        Palette_Load(file + 0x988, 0x220, 0x40);
    GFX_BACK();
    GFX_TO(SECOND_MAP);
        Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
        Font_LoadDigits(0);
        Mem_Copy32(MAIN_OBJ_VRAM + 0xc00, data_027c0280->objVram + 0xc00, 0xa00);
        Palette_Load(file + 0x988, 0x220, 0x40);
    GFX_BACK();
    Mem_Free(file);

    file = File_Load(gCoRankSurvivalPath);
    GFX_TO(MAIN_MAP);
        Data_Unpack(file + 0xa74, data_027c0280->objVram);
        Palette_Load(file + 0xf58, 0x200, 0x20);
        Data_Unpack(file + 0xc30, data_027c0280->objVram + 0x1600);
        Palette_Load(file + 0xe58, 0x260, 0x20);
        Data_Unpack(file + 0xcd8, data_027c0280->objVram + 0x2880);
    GFX_BACK();
    GFX_TO(SECOND_MAP);
        Mem_Copy32(MAIN_OBJ_VRAM, data_027c0280->objVram, 0xc00);
        Palette_Load(file + 0xf58, 0x200, 0x20);
        Mem_Copy32(MAIN_OBJ_VRAM + 0x1600, data_027c0280->objVram + 0x1600, 0x200);
        Palette_Load(file + 0xe58, 0x260, 0x20);
    GFX_BACK();
    Mem_Free(file);

    file = File_Load(File_LocalizePath(gCoRankLangPath));
    GFX_TO(MAIN_MAP);
        Data_Unpack(file, data_027c0280->objVram + 0x2e80);
    GFX_BACK();
    Mem_Free(file);

    file = File_Load(gCoRankObjPath);
    GFX_TO(MAIN_MAP);
        switch (Survival_GetRunGrade()) {
        case 5:
            Data_Unpack(file, data_027c0280->objVram + 0x1f80);
            Palette_Load(file + 0x152c, 0x2a0, 0x20);
            break;
        case 4:
            Data_Unpack(file + 0x56c, data_027c0280->objVram + 0x1f80);
            Palette_Load(file + 0x154c, 0x2a0, 0x20);
            break;
        case 3:
            Data_Unpack(file + 0x9e4, data_027c0280->objVram + 0x1f80);
            Palette_Load(file + 0x156c, 0x2a0, 0x20);
            break;
        default:
            Data_Unpack(file + 0xd54, data_027c0280->objVram + 0x1f80);
            Palette_Load(file + 0x158c, 0x2a0, 0x20);
            break;
        }
        Data_Unpack(file + 0xfb4, data_027c0280->objVram + 0x2780);
    GFX_BACK();
    GFX_TO(SECOND_MAP);
        Data_Unpack(file + 0x13c4, data_027c0280->objVram + 0x1800);
        Palette_Load(file + 0x14cc, 0x280, 0x20);
    GFX_BACK();
    Mem_Free(file);

    file = File_Load(File_LocalizePath(gCoRankCtcLangPath));
    Mem_Copy32(file, gMenuState, 0x160);
    Mem_Free(file);

    GFX_TO(MAIN_MAP);
        Icon_LoadTo(139, data_027c0280->objVram, 192);
        for (i = 0; i < 10; i++) {
            Icon_LoadTo(i + 140, data_027c0280->objVram, 200 + i * 2);
        }
        Icon_LoadTo(201, data_027c0280->objVram, 224);
        Icon_LoadTo(213, data_027c0280->objVram, 220);
        Icon_LoadTo(182, data_027c0280->objVram, 228);
        for (i = 0; i < 10; i++) {
            Icon_LoadTo(i + 150, data_027c0280->objVram, 232 + i * 2);
        }
        IconBank_LoadPalette(4, 20);
        task->hasBackCo = STATE->players[1].back.id != 0;
        Co_LoadGraphicsAndPalette(STATE->players[1].front.id & PLAYER_CO_ID_MASK, 0x400, 6);
        if (task->hasBackCo) {
            Co_LoadGraphicsAndPalette(STATE->players[1].back.id & PLAYER_CO_ID_MASK, 0x5b0, 7);
        }
    GFX_BACK();
    func_01ff9b48();
    Menu_Reset();
}
