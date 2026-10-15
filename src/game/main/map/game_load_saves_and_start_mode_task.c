#include "types.h"
#include "game/Unk_02080440.h"

void Save_LoadProfiles(void);
void Save_LoadProgress(void);
void Save_LoadSurvivalState(void);
void func_0210322c(void);
void func_0210372c(void);

/* Loads the saved profiles and progress the game mode needs, then starts
 * its task. The war room starts func_0210322c when coSelectPhase is set and
 * func_0210372c otherwise. player is not used. */
void Game_LoadSavesAndStartModeTask(int player, int coSelectPhase) {
  switch (MAP_STATE->gameMode) {
    case GAME_MODE_CAMPAIGN:
      Save_LoadProfiles();
      Save_LoadProgress();
      func_0210372c();
      break;
    case GAME_MODE_WAR_ROOM:
      Save_LoadProgress();
      if (coSelectPhase != 0) {
        func_0210322c();
      } else {
        func_0210372c();
      }
      break;
    case GAME_MODE_VERSUS:
      func_0210372c();
      break;
    case GAME_MODE_SURVIVAL:
      Save_LoadSurvivalState();
      Save_LoadProgress();
      func_0210372c();
      break;
  }
}
