#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"
#include "game/screen_other_map.h"

extern Unk_02080440_50 data_02169f48[];

#define STATE_RECORD(n) (data_027c027c->unk_50[n])

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

#define OTHER_SCREEN (&data_027c0288.unk_00)[1 - data_027c0284]

/* Which record the current game state gets: 2 on the main screen; 3 or 4
 * by Scene_GetKind; in a two-map battle 1 when the other state's unk_30 is
 * 0; else 0. */
static inline int GetRecordKind(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u32 otherField;

  if (data_027c027c == data_027c0288.unk_00) {
    return (data_027c027c == data_027c0288.unk_00) ? 2 : 1;
  }
  switch (Scene_GetKind()) {
    case 2:
      return 3;
    case 3:
      return 4;
    default:
      if (data_027c027c->unk_48[data_027c0284]->secondMapId != 0) {
        SWITCH_TO(OTHER_SCREEN);
        otherField = data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_30;
        SWITCH_BACK();
        if (otherField == 0) {
          return 1;
        }
      }
      return 0;
  }
}

#define SELECT_RECORD(screen)                           \
  {                                                     \
    Unk_027c0280* saved0280;                            \
    Unk_02080440* saved027c;                            \
    int kind;                                           \
                                                        \
    SWITCH_TO(screen);                                  \
    kind = GetRecordKind();                             \
    STATE_RECORD(data_027c0284) = &data_02169f48[kind]; \
    SWITCH_BACK();                                      \
  }

/* Gives the game state of each screen its record from data_02169f48. */
void Screens_SelectConfigs(void) {
  SELECT_RECORD(data_027c0288.unk_00)
  SELECT_RECORD(data_027c0288.unk_04)
}
