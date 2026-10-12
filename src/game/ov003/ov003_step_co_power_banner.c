#include "types.h"
#include "game/co_data.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_0235dd38.h"

extern Unk_027c0280* data_027c0280;
extern u8 gMenuState[][0x3600];
extern u8 (*data_ov003_02365b10)[0x3600];

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Mem_Copy32(const void* src, void* dst, u32 size);
int CombatCo_IdFromNumber(int mode);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
const char* Text_Get(u32 textId);
int BreakText_Load(const char* text, int a, int b, int c);
void Task_ResumeScript(Unk_ov003_0235dd38* task);

/* High byte of Ov003State.unk_1d6. */
#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

/* Script command that runs for 8 steps (Unk_ov003_0235dd38.frame counts them, but not
 * while the byte at 0x1d7 of Ov003State is set) and then resumes the script.
 * Task.unk_52 is a row number. The blend value follows an ease from 0 to 8
 * over the steps. Step 1 copies the 0x3600-byte image of that row into
 * object VRAM (rows 1 and 2 come from gMenuState, the others from
 * data_ov003_02365b10). Step 2 loads the palette of the row's CO. Step 3
 * starts printing the name of that CO's CO Power. */
void Ov003_StepCoPowerBanner(Unk_ov003_0235dd38* task) {
  int row;

  if (STATE_BYTE_1D7 == 0) {
    task->frame++;
  }
  data_027c0280->blendEvy = Math_Ease(0, 0, 8, task->frame, 8);
  switch (task->frame) {
    case 1:
      row = task->unk_52;
      Mem_Copy32(
          (row - 1 < 2) ? gMenuState[row - 1] : data_ov003_02365b10[row - 3],
          data_027c0280->objVram + 0xb1c0, 0x3600);
      break;
    case 2:
      row = task->unk_52;
      Co_LoadArmyPalette(
          CombatCo_IdFromNumber(data_ov003_02364928->rows[row - 1].co), 19,
          row);
      break;
    case 3:
      row = task->unk_52;
      /* Needed to match: indexing from &gCoTable[0].blocks[POWER_CO], as
         * DrawSelectedCoPowerText does. */
      task->count = BreakText_Load(
          Text_Get(*(u32*)&((CoRecord*)&gCoTable[0].blocks[POWER_CO])
                        [CombatCo_IdFromNumber(
                             data_ov003_02364928->rows[row - 1].co) &
                         0x7f]
                            .name),
          0x73e, 4, 0);
      break;
  }
  if (task->frame == 8) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
