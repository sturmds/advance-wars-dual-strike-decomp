#include "types.h"
#include "game/screen_other_map.h"

void GameFlagsMid_Backup(void);
void LinkMenu_Start(void);

/* Copies the game flags (GameFlagsMid_Backup), then opens the link menu with
 * unk_8b 2: its own login screen tiles and title text, and a limit of
 * 2 players in Link_ResetState. */
void LinkMenu_StartMode2(void) {
  GameFlagsMid_Backup();
  data_027c027c->unk_48[data_027c0284]->unk_8b = 2;
  LinkMenu_Start();
}
