#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_help.h"
#include "game/play_records.h"
#include "game/screen_other_map.h"
#include "game/song.h"

void Player_PlayTheme(int playerIndex);
void PopupMenu_Refresh(void);

#define CURRENT_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* Battle menu handler (table entries at 0x02165d80 and 0x02165da0): turns the
 * music on or off. On a two-map battle the other map follows when its
 * unk_1c4->unk_30 is 0. Starts the current player's theme or fades the song
 * out, draws the matching help text, refreshes the menu and saves the setting
 * in gPlayRecords. Does nothing while a song fades out. */
void Menu_ToggleMusic(int a0, int a1, u32 flags) {
  MenuHelpPair help = gMusicMenuHelpFns;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  u8 musicReady;
  u32 otherUnk30;

  if (Sound_IsSongFadingOut()) {
    return;
  }
  SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
  MAP_STATE->musicReady = 1 - MAP_STATE->musicReady;
  musicReady = MAP_STATE->musicReady;
  SWITCH_BACK(savedGfx, savedGame);
  if (MAP_STATE->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
    otherUnk30 = MAP_STATE->unk_1c4->unk_30;
    SWITCH_BACK(savedGfx, savedGame);
    if (otherUnk30 == 0) {
      SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
      MAP_STATE->musicReady = musicReady;
      SWITCH_BACK(savedGfx, savedGame);
    }
  }
  switch (MAP_STATE->musicReady) {
    case 0:
      Sound_FadeOutSongOver(1);
      break;
    case 1:
      Player_PlayTheme((u8)MAP_STATE->currentPlayer);
      break;
  }
  help.fn[MAP_STATE->musicReady](a0, a1, flags);
  PopupMenu_Refresh();
  gPlayRecords.unk_0e = MAP_STATE->musicReady;
}
