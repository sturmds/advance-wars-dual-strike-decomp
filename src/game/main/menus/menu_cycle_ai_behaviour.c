#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/play_records.h"

/* Same arguments as the menu handler; the help text functions it points to
 * (BattleMenu_DrawAiStrikeHelp, BattleMenu_DrawAiAssaultHelp, BattleMenu_DrawAiGeneralHelp, BattleMenu_DrawAiDefenseHelp) take none. */
typedef void (*MenuHelpFn)(int a0, int a1, u32 flags);

/* Copied to the stack as a whole: one help text function per behaviour. */
typedef struct MenuHelpFour {
    MenuHelpFn fn[4];
} MenuHelpFour;

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void PopupMenu_Refresh(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuHelpFour gAiBehaviourMenuHelpFns;
extern PlayRecords gPlayRecords;
extern u32 gLinkPlay;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Battle menu handler (table entries at gBattleMenuItems + 0xa0 to + 0x100,
 * one per value): steps the current player's aiBehaviour through 0-3 on both
 * screens, draws the matching help text, refreshes the menu and saves the
 * value in gPlayRecords for campaign or War Room. In link play it also sends
 * the change as unit action 27. */
void Menu_CycleAiBehaviour(int a0, int a1, u32 flags)
{
    MenuHelpFour help = gAiBehaviourMenuHelpFns;
    Unk_02080440 *savedGame = data_027c027c;
    u8 behaviour;
    u8 player;
    Unk_027c0280 *savedGfx;

    player = GAME->currentPlayer;
    behaviour = GAME->players[player].aiBehaviour + 1;
    if (behaviour >= 4) {
        behaviour = AI_BEHAVIOUR_STRIKE;
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    GAME->players[player].aiBehaviour = behaviour;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    GAME->players[player].aiBehaviour = behaviour;
    Screen_SetCurrent(savedGame);
    Screen_SetCurrentGfx(savedGfx);
    help.fn[behaviour](a0, a1, flags);
    PopupMenu_Refresh();
    if (GAME->gameMode != GAME_MODE_CAMPAIGN) {
        if (GAME->gameMode == GAME_MODE_WAR_ROOM) {
            gPlayRecords.warRoomAiBehaviour = behaviour;
        }
    } else {
        gPlayRecords.campaignAiBehaviour = behaviour;
    }
    if (gLinkPlay != 0) {
        UnitAction_Send(27, 0, player, behaviour);
    }
}
