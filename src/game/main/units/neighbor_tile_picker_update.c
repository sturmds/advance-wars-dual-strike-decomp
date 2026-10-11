#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
/* 5 bytes per direction 0-3 from data_02166378: mask bit, dx, dy, then a
 * one-step path. These two point at dx and dy. */
extern s8 data_02166379[];
extern s8 data_0216637a[];

u32 Keypad_ReadNextPrev(void);
u32 Keypad_ReadConfirmCancel(void);
int NeighborDir_Next(int direction, int mask); /* next direction in mask */
int NeighborDir_Prev(int direction, int mask); /* previous direction in mask */
void MapTouch_UpdateNeighborCursor(int *direction, int mask);
u32 MapTouch_ReadNeighborTileTap(int mask);
void Sound_PlaySe(u32 soundId);
void Map_ScrollViewToTile(int arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Bits Keypad_ReadNextPrev returns for the d-pad. */
#define INPUT_NEXT 8
#define INPUT_PREV 4
/* Bits Keypad_ReadConfirmCancel returns: A and B. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL  0x20

/* Picker for one of the four tiles around the acting unit's destination,
 * limited to the directions whose bit is in mask (used by unload). The d-pad and
 * touch move *direction, with sound 0x10000 on a change; the view target
 * follows the picked tile. Returns INPUT_CANCEL, INPUT_CONFIRM or 0. */
u32 NeighborTilePicker_Update(int *direction, int mask)
{
    int picked = *direction;
    int old = *direction;
    u16 keys = Keypad_ReadNextPrev();
    u16 input;

    if (keys & INPUT_NEXT) {
        picked = NeighborDir_Next(picked, mask);
    }
    if (keys & INPUT_PREV) {
        picked = NeighborDir_Prev(picked, mask);
    }
    MapTouch_UpdateNeighborCursor(&picked, mask);
    if (picked != old) {
        *direction = picked;
        Sound_PlaySe(0x10000);
    }
    STATE->cursorX = STATE->actingUnitDestX + data_02166379[picked * 5];
    STATE->cursorY = STATE->actingUnitDestY + data_0216637a[picked * 5];
    Map_ScrollViewToTile(1);
    input = Keypad_ReadConfirmCancel();
    input |= MapTouch_ReadNeighborTileTap(mask);
    if (input & INPUT_CANCEL) {
        return INPUT_CANCEL;
    }
    if (input & INPUT_CONFIRM) {
        return INPUT_CONFIRM;
    }
    return 0;
}
