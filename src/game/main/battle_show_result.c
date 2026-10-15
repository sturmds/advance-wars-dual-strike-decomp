#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

BOOL Player_IsInGame(u8 player);
int func_020d69c0(int parentTask);
int func_020d69e4(int winner, int parentTask);

extern u32 gLinkPlay;

/* End of a battle: picks the winning player slot and starts the win result
 * (func_020d69e4) or the loss result (func_020d69c0). Outside Versus the
 * result follows player 1. In Versus the loss result runs while players of
 * two teams are left; otherwise the last human player in the game wins, or
 * the last AI player when no slot is human. */
void Battle_ShowResult(int parentTask) {
  int i;
  int j;
  u8 human;
  u8 ai;
  u8 humanLeft;
  u8 rivalsLeft;
  u8 aiLeft;
  Player* player;

  /* Needed to match: the index is loaded before the base. */
  switch (STATE_IDX_FIRST->gameMode) {
    case GAME_MODE_CAMPAIGN:
    case GAME_MODE_WAR_ROOM:
    case GAME_MODE_SURVIVAL:
      if (Player_IsInGame(1)) {
        func_020d69e4(1, parentTask);
      } else {
        func_020d69c0(parentTask);
      }
      return;
    case GAME_MODE_VERSUS:
      aiLeft = 0;
      humanLeft = 0;
      ai = 0;
      human = 0;
      rivalsLeft = 0;
      player = &MAP_STATE->players[1];
      for (i = 1; i <= PLAYER_SLOT_LAST; player++, i++) {
        if (player->controller != PLAYER_CONTROLLER_NONE) {
          if (MAP_STATE->players[i].defeated == 0) {
            for (j = 1; j <= PLAYER_SLOT_LAST; j++) {
              if (i != j &&
                  MAP_STATE->players[j].controller != PLAYER_CONTROLLER_NONE &&
                  MAP_STATE->players[j].defeated == 0 &&
                  MAP_STATE->players[i].team != MAP_STATE->players[j].team) {
                rivalsLeft = 1;
                break;
              }
            }
          }
          if (rivalsLeft == 1) {
            break;
          }
        }
      }
      if (rivalsLeft == 1) {
        func_020d69c0(parentTask);
        return;
      }

      for (i = 1; i <= PLAYER_SLOT_LAST; i++) {
        switch (MAP_STATE->players[i].controller) {
          case PLAYER_CONTROLLER_NONE:
            break;
          case PLAYER_CONTROLLER_HUMAN:
            human = i;
            if (Player_IsInGame(i)) {
              humanLeft = i;
            }
            break;
          case PLAYER_CONTROLLER_AI:
            ai = i;
            if (Player_IsInGame(i)) {
              aiLeft = i;
            }
            break;
          default:
            if (Player_IsInGame(i)) {
              aiLeft = i;
            }
            break;
        }
      }

      if (gLinkPlay != 0) {
        if (human != 0) {
          if (humanLeft != 0) {
            func_020d69e4(humanLeft, parentTask);
          } else {
            func_020d69c0(parentTask);
          }
        } else {
          if (aiLeft != 0) {
            func_020d69e4(aiLeft, parentTask);
          } else {
            func_020d69c0(parentTask);
          }
        }
      } else if (ai == 0) {
        func_020d69e4(humanLeft, parentTask);
      } else if (human == 0) {
        func_020d69e4(aiLeft, parentTask);
      } else if (humanLeft != 0) {
        func_020d69e4(humanLeft, parentTask);
      } else {
        func_020d69c0(parentTask);
      }
      return;
  }
}
