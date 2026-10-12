#include "types.h"
#include "game/screen_other_map.h"

void Gfx_LoadBmap09f(void* target, int a, int b);
void* EventScript_Start(const void* script);

extern u8* data_02297790;
extern const u8 data_02161980[];

/* Same work as func_0208c138: points data_02297790 at BG VRAM + 0x10000. Then
 * loads graphics there and starts the event script data_02161980. */
void CombatIntro_StartEventScript(void) {
  data_02297790 = data_027c0280->bgVram + 0x10000;
  Gfx_LoadBmap09f(data_02297790, 0, 0);
  EventScript_Start(data_02161980);
}
