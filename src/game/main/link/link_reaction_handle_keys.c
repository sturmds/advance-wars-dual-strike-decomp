#include "types.h"
#include "game/keypad.h"
#include "game/link_reaction_queue.h"

BOOL EventScript_IsRunning(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
void LinkReaction_Send(void* data);
void LinkReaction_StartCooldown(void);

/* Sends a reaction when the local player presses A, B, X, Y, L or R
 * (buttons 0 to 5), unless the wait after the last one is still running
 * or an event script is. */
void LinkReaction_HandleKeys(void) {
  LinkReaction reaction;

  if (gLinkReactionCooldown != 0) {
    gLinkReactionCooldown--;
    return;
  }
  if (EventScript_IsRunning()) {
    return;
  }
  reaction.slot = Link_FindSlotOfPlayer(Link_GetOwnPlayer());
  if (gKeypad->pressed & PAD_BUTTON_A) {
    reaction.button = 0;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    reaction.button = 1;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  } else if (gKeypad->pressed & PAD_BUTTON_X) {
    reaction.button = 2;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  } else if (gKeypad->pressed & PAD_BUTTON_Y) {
    reaction.button = 3;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  } else if (gKeypad->pressed & PAD_BUTTON_L) {
    reaction.button = 4;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  } else if (gKeypad->pressed & PAD_BUTTON_R) {
    reaction.button = 5;
    LinkReaction_Send(&reaction);
    LinkReaction_StartCooldown();
  }
}
