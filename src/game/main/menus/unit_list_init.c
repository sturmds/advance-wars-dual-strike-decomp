#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/unit_list_task.h"

void *Mem_Alloc(u32 size);
int UnitList_ClearAndCount(void);
void UnitList_Build(int sortMode, int reverse);
void UnitList_AddButtons(void);
void func_01ff8c84(int layer);
void Icon_LoadTo(int iconId, u8 *vram, int tile);
void IconBank_LoadPalette(int bank, int paletteSlot);
void File_ReadAt(void *dest, const char *path, u32 offset, u32 size);
void TpIcon2_LoadTo(u8 *dest);
void File_UnpackTo(void *dest, const char *fileName);
void Font_LoadDigits(int baseTile);
const char *Text_Get(u32 textId);
void Text_PrintStringImmediate(s16 x, s16 y, u16 *map, const char *text, u16 a, u16 b);
void Number_Draw(s16 x, s16 y, u16 *map, u32 value, u16 entryBits, u16 digitSet);
void Gfx_RequestLayerCopy(int layer);
void UnitList_DrawHeadings(void);
void UnitList_DrawRows(int scroll);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern MenuState gMenuState;
extern u8 *gUnitListRows;
extern const char data_0215fb10[];
extern const char data_0215fb28[];
extern const u32 data_02167c9c[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Sets up the unit list menu task: allocates and builds the list (type
 * order, not reversed), loads the icons, the 16-colour cycle palette
 * (32 bytes from offset 32 of data_0215fb10 into gMenuState) and the
 * background map, prints the text that data_02167c9c gives for the current
 * player's army colour and the unit count, draws the rows and adds the
 * buttons. */
void UnitList_Init(UnitListTask *task)
{
    u16 *map;

    gUnitListRows = Mem_Alloc(64);
    task->unitCount = UnitList_ClearAndCount();
    task->sortMode = 0;
    task->reverse = 0;
    task->scroll = 0;
    task->topRow = 0;
    task->scrollDelay = 0;
    task->pickedSlot = 0;
    task->unk_2f = 10;
    task->unk_30 = 10;
    task->unk_34 = 0;
    task->unk_38 = 0;
    UnitList_Build(task->sortMode, task->reverse);
    func_01ff8c84(0);
    func_01ff8c84(1);
    Icon_LoadTo(GAME->players[GAME->currentPlayer].armyColour + 74, data_027c0280->objVram, 404);
    IconBank_LoadPalette(1, 22);
    Icon_LoadTo(1, data_027c0280->objVram, 414);
    IconBank_LoadPalette(0, 21);
    Icon_LoadTo(138, data_027c0280->objVram, 412);
    IconBank_LoadPalette(4, 20);
    File_ReadAt(&gMenuState, data_0215fb10, 32, 32);
    TpIcon2_LoadTo(data_027c0280->objVram + 0x3440);
    File_UnpackTo(data_027c0280->layer1Map, data_0215fb28);
    Font_LoadDigits(0);
    map = data_027c0280->layer0Map;
    Text_PrintStringImmediate(5, 1, map,
                              Text_Get(data_02167c9c[GAME->players[GAME->currentPlayer].armyColour]), 0, 0);
    Number_Draw(30, 1, data_027c0280->layer0Map, task->unitCount, 0, 0);
    UnitList_DrawHeadings();
    UnitList_DrawRows(task->scroll);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    UnitList_AddButtons();
}
