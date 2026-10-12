#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_0201e954(void);
void func_020ddb38(void);
void Link_SendWeather(int nextWeather);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Menu handler (table entry at 0x02165d00): func_020ddb38 picks the next turn's
 * weather; with gLinkPlay set, it is sent through Link_SendWeather. */
void Menu_PickNextWeather(void) {
  func_0201e954();
  func_020ddb38();
  if (gLinkPlay != 0) {
    Link_SendWeather(GAME->unk_1c4->nextWeather);
  }
}
