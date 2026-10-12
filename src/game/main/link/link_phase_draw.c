#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Palette_Load(const void* src, u32 offset, u32 size);
int MainLoop_GetFrameCount(void);
void Sprite_QueuePicture192x160(s16 x, int y, int attr, int tile, int list);
void LinkPhase_DrawPlayerMarker(int player);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;

/* Per-frame draw of the link phase screen (set up by LinkPhase_InitScreen;
 * both are referenced from the screen table at data_0215f824). Draws each
 * player's marker in panel order, cycles one colour (palette buffer offset
 * 0x234) through the 16 colours at the start of gMenuState, adds a 192x160
 * picture when the battle has two maps, and another on the second game
 * state's screen. */
void LinkPhase_Draw(void) {
  int panel;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  for (panel = 0; panel < gMenuState.link.playerCount; panel++) {
    LinkPhase_DrawPlayerMarker(gMenuState.link.panelPlayers[panel]);
  }
  Palette_Load((u16*)&gMenuState + ((MainLoop_GetFrameCount() >> 2) & 15),
               0x234, 2);
  if (data_027c027c->unk_48[data_027c0284]->twoMaps == 1) {
    Sprite_QueuePicture192x160(128, 96, 0x3400, 60, 1);
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Sprite_QueuePicture192x160(128, 96, 0x2000, 0x200, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
