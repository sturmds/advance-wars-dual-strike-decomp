#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

void func_020c4024(void);
void func_020c40d0(void);
void MapView_ScrollStep(int a);
void SidePanel_Place(int mode);

/* Runs three calls only for a human current player; the purpose of the calls is not clear yet. */
void func_020800c0(void) {
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
      PLAYER_CONTROLLER_AI) {
    func_020c4024();
    func_020c40d0();
    MapView_ScrollStep(4);
  }
  SidePanel_Place(SIDE_PANEL_SNAP_AWAY);
}
