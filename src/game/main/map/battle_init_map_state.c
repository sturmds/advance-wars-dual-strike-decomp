#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void Battle_ResetTurnState(void);
void TileCodeTable_Load(void);
void Map_LoadVariant(int mapId, int variant);
void func_020c4fa0(void);
void Survival_LoadFunds(void);
void Skills_SetupForBothMaps(int gameMode, int mapId);
void MovementCost(void);
void func_020c4f20(void);
void MapView_CentreOnPlayer1Hq(void);
void CurrentPlayer_SyncState(void);
void CurrentPlayer_StartTurn(void);

/* Battle start: resets the turn state, loads the map in the state (the map's
 * own variant 0) and sets up everything that depends on it: movement costs
 * and the current player. */
void Battle_InitMapState(void) {
  Battle_ResetTurnState();
  TileCodeTable_Load();
  Map_LoadVariant(STATE->unk_1c4->mapId, 0);
  func_020c4fa0();
  Survival_LoadFunds();
  Skills_SetupForBothMaps(STATE->gameMode, STATE->unk_1c4->mapId);
  MovementCost();
  func_020c4f20();
  MapView_CentreOnPlayer1Hq();
  CurrentPlayer_SyncState();
  CurrentPlayer_StartTurn();
}
