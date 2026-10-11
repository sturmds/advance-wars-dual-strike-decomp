#include "types.h"
#include "sdk/mb.h"

extern DlPlayAdvertProgress data_022ace4c;

BOOL MbBeacon_SelectNextGame(void);
void MbBeacon_PlanPieces(void);
void MbBeacon_SendPiece(u32 ggid, u16 tgid, u32 attribute);
void MbBeacon_EndGame(void);
void MbBeacon_SendPlayers(u32 ggid, u16 tgid, u32 attribute);

/* Runs the beacon state machine until it has sent one beacon. */
void MbBeacon_Step(u32 ggid, u16 tgid, u32 attribute)
{
    for (;;) {
        switch (data_022ace4c.state) {
        case DLPLAY_ADVERT_STEP_OFF:
        case DLPLAY_ADVERT_STEP_PICK:
            if (!MbBeacon_SelectNextGame()) {
                return;
            }
            break;
        case DLPLAY_ADVERT_STEP_SPLIT:
            MbBeacon_PlanPieces();
            break;
        case DLPLAY_ADVERT_STEP_EMIT:
            MbBeacon_SendPiece(ggid, tgid, attribute);
            return;
        case DLPLAY_ADVERT_STEP_DONE:
            MbBeacon_EndGame();
            break;
        case DLPLAY_ADVERT_STEP_ROSTER:
            MbBeacon_SendPlayers(ggid, tgid, attribute);
            return;
        case 6:
            break;   /* no work in this state: the loop reads the state again */
        }
    }
}
