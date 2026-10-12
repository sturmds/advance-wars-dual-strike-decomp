#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActivatePower(int player, int power);
void func_020c4018(void);

void UnitAction_ActivateTagPower(void) {
  if (data_027c027c->unk_48[data_027c0284]
          ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
          .controller == PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.tagPowersActivated < 999999) {
      gPlayRecords.tagPowersActivated++;
    }
  }
  data_027c027c->unk_48[data_027c0284]
      ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
      .tagPowerHalf = TAG_POWER_FRONT;
  ActivatePower(data_027c027c->unk_48[data_027c0284]->currentPlayer,
                POWER_SUPER);
  func_020c4018();
}
