#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_portrait_task.h"
#include "game/task.h"

void Gfx_RequestLayerCopy(int a);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Text_PrintImmediate(int x, int y, void* target, u32 text, int a, int b);
void Text_PrintById(int x, int y, void* target, u32 text, int a, int b);
CoPortraitTask* Task_FindByScript(void* key);
void Bg_ClearRect(void* target, int x, int y, int w, int h);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8
    gCombatModeCos[]; /* CO ids: Andy, Max, Sami, Flak, Lash, Adder, Hawke */
extern TaskScriptCmd data_021613d8[];

/* Draws the CO Power name and a second text line for the CO selected in
 * the Combat mode menu (the CO portrait task's selectedSlot). */
void DrawSelectedCoPowerText(void) {
  CoPortraitTask* portrait;
  u32 text;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  portrait = Task_FindByScript(data_021613d8);
  switch (gCombatModeCos[portrait->selectedSlot]) {
    case CO_ANDY:
      text = 0x44000003;
      break;
    case CO_MAX:
      text = 0x44000004;
      break;
    case CO_SAMI:
      text = 0x44000005;
      break;
    case CO_FLAK:
      text = 0x44000006;
      break;
    case CO_LASH:
      text = 0x44000007;
      break;
    case CO_ADDER:
      text = 0x44000008;
      break;
    case CO_HAWKE:
      text = 0x44000009;
      break;
  }
  Bg_ClearRect(data_027c0280->layer0Map, 17, 13, 15, 8);
  /* Needed to match: indexing from &gCoTable[0].blocks[POWER_CO] keeps
     * the 0x120 offset in the literal pool instead of the load. TextRefs
     * are passed as one u32. */
  Text_PrintImmediate(
      19, 13, data_027c0280->layer0Map,
      *(u32*)&(
           (CoRecord*)&gCoTable[0]
               .blocks[POWER_CO])[gCombatModeCos[portrait->selectedSlot] & 0x7f]
           .name,
      0, 0);
  Text_PrintById(17, 15, data_027c0280->layer0Map, text, 0, 0x340);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
