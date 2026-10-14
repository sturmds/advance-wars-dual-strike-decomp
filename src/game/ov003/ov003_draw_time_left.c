#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"
#include "game/oam_cells.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8* data_02160abc;

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Ov003_DefeatTeam(int a);

#define DIGIT_TILE(digit) (0x578 + (digit) * 2)
#define COLON_TILE 0x58c

/* Draws the time left as m:ss with digit sprites at the top right of the screen.
 * The time limit is preset->timeLimitMinutes; unk_1cc counts up 60 per second.
 * In the last second, Ov003_DefeatTeam(1) runs once per call. */
void Ov003_DrawTimeLeft(void) {
  Ov003State* state = data_ov003_02364928;
  u32 limitFrames;
  u32 framesLeft;
  int seconds;
  int minutes;
  int minuteTens;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (state->preset->timeLimitMinutes == 0) {
    return;
  }
  if (data_02160ac0->unk_7c != 0) {
    return;
  }
  limitFrames = state->preset->timeLimitMinutes * 3600;
  if (limitFrames - 60 <= state->unk_1cc) {
    if (state->rows[0].active && data_02160abc[10] == 0) {
      Ov003_DefeatTeam(1);
    }
    framesLeft = 0;
  } else {
    framesLeft = limitFrames - state->unk_1cc;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  seconds = HwDiv32(framesLeft, 60);
  minutes = HwDiv32(seconds, 60) & 255;
  seconds = HwMod32(seconds, 60) & 255;
  Sprite_QueueAt(0, 248, 0, &data_027c0038, 0x2000,
                 DIGIT_TILE(HwMod32(seconds, 10)));
  Sprite_QueueAt(0, 240, 0, &data_027c0038, 0x2000,
                 DIGIT_TILE(HwDiv32(seconds, 10)));
  Sprite_QueueAt(0, 232, 0, &data_027c0038, 0x2000, COLON_TILE);
  Sprite_QueueAt(0, 224, 0, &data_027c0038, 0x2000,
                 DIGIT_TILE(HwMod32(minutes, 10)));
  minuteTens = HwDiv32(minutes, 10);
  if (minuteTens != 0) {
    Sprite_QueueAt(0, 216, 0, &data_027c0038, 0x2000, DIGIT_TILE(minuteTens));
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
