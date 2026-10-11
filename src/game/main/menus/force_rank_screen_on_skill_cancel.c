#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Sound_PlaySe(int id);
void ForceRankScreen_DrawSlotRow(int x, int y, int textTile, int skill);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* onCancel of a skill button: empties the selected slot, redraws its row
 * on the second screen and moves the selection back one slot. */
void ForceRankScreen_OnSkillCancel(MenuButton *button)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;

    Sound_PlaySe(0x10009);
    gForceRankScreen->chosenSkills[gForceRankScreen->selectedSlot] = 0;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    ForceRankScreen_DrawSlotRow(1, 15 + gForceRankScreen->selectedSlot * 2, 256 + gForceRankScreen->selectedSlot * 64,
                                gForceRankScreen->chosenSkills[gForceRankScreen->selectedSlot]);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    if (gForceRankScreen->selectedSlot != 0) {
        gForceRankScreen->selectedSlot--;
    }
}
