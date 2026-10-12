#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Song id, from romhacking.net doc 890 (RadioTails). */
#define SONG_CO_SELECT 0x21

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_IsCoSelectPhase(void);
void Sound_PlaySong(int song);
int Player_GetCoThemeSong(int player);
void PlayCoPowerTheme(int player);
void PlaySuperPowerTheme(int player);
void PlayTagPowerTheme(int player);

/* TRUE when the player has a back CO and a tag power is on. */
static inline BOOL Player_IsTagPowerOn(Player* player) {
  BOOL tag = FALSE;
  if (player->back.id != 0) {
    BOOL half = TRUE;
    if (player->tagPowerHalf != TAG_POWER_FRONT &&
        player->tagPowerHalf != TAG_POWER_BACK) {
      half = FALSE;
    }
    if (half) {
      tag = TRUE;
    }
  }
  return tag ? TRUE : FALSE;
}

/* Starts the music for a player: CO Select while a CO is being picked, else
 * the theme for the front CO's power state. Player_GetCoThemeSong gives the front
 * CO's own theme from gCoTable. */
void Player_PlayTheme(int playerIndex) {
  Player* player;

  if (Map_IsCoSelectPhase()) {
    Sound_PlaySong(SONG_CO_SELECT);
    return;
  }
  /* needed to match: the byte offset comes first, or the two global loads
     * swap registers */
  player = (Player*)(playerIndex * sizeof(Player) +
                     (u32)data_027c027c->unk_48[data_027c0284]->players);
  switch (player->front.powerState) {
    case POWER_NONE:
    default:
      Sound_PlaySong(Player_GetCoThemeSong(playerIndex));
      break;
    case POWER_CO:
      PlayCoPowerTheme(playerIndex);
      break;
    case POWER_SUPER:
      if (Player_IsTagPowerOn(player) == TRUE) {
        PlayTagPowerTheme(playerIndex);
      } else {
        PlaySuperPowerTheme(playerIndex);
      }
      break;
  }
}
