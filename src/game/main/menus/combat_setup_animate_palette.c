#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);

/* MenuState.combat.unk_1400. Needed to match: the compiler adds the offset to
 * the address of gMenuState at run time. */
#define COMBAT_PALETTE \
  ((u16*)((u8*)&gMenuState + offsetof(MenuState, combat.unk_1400)))

/* Loads one colour of the 16-colour palette in the Combat setup layout (res_linkcommon) into
 * byte offset 564 of the palette of both screens, stepping every 4 frames. */
void CombatSetup_AnimatePalette(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Palette_Load(&COMBAT_PALETTE[(MainLoop_GetFrameCount() >> 2) & 15], 564, 2);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Palette_Load(&COMBAT_PALETTE[(MainLoop_GetFrameCount() >> 2) & 15], 564, 2);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
