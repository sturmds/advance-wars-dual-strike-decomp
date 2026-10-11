#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_0201d320 data_022977f4[];
extern s16 gEventTextWindowPos;    /* screen the picture goes on: 0 or 1 */
extern void *data_01ff8f64[]; /* ITCM table of two functions, one per screen */
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_0201dc44(int index);
int Index_FromNumber(int number);
void func_0201dc38(void *arg);
void EventScript_StartScreenSlideOut(int index);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Record picture value with this bit set: the low bits name a player slot, and the picture is
 * that player's front CO with the mood from the record's unk_0a. */
#define PICTURE_FROM_PLAYER    0x8000
#define PICTURE_PLAYER_MASK    0x7fff
#define PICTURE_NONE           (-1)   /* keep the current picture */
#define PICTURE_PLAYER_CURRENT 5      /* slot value meaning the current player */

/* Helper of EventCmd_ShowPicture for event slot index. Reads the screen from the record's
 * unk_0c and the picture (CO id in the low byte, mood index in the high byte) from unk_08,
 * stores the picture in data_022977a4.unk_02, passes the screen's entry of data_01ff8f64 to
 * func_0201dc38, and starts the slide-out on that screen. */
void EventScript_ShowPicture(s16 index)
{
    Unk_0201d320_Entry *record;
    s16 picture;
    int slot;
    s16 mood;

    func_0201dc44(index);
    record = data_022977f4[index].unk_00;
    gEventTextWindowPos = record->unk_0c;
    picture = record->unk_08;
    if (picture != PICTURE_NONE) {
        if (picture & PICTURE_FROM_PLAYER) {
            slot = picture & PICTURE_PLAYER_MASK;
            if (slot == PICTURE_PLAYER_CURRENT) {
                slot = STATE->currentPlayer;
            }
            /* the record's mood is 1-based */
            mood = Index_FromNumber(record->unk_0a);
            /* needed to match: the s16 cast gives smlabb for the index */
            data_022977a4.unk_02 = (STATE->players[(s16)slot].front.id & PLAYER_CO_ID_MASK) | (mood << 8);
        } else {
            data_022977a4.unk_02 = picture;
        }
    }
    func_0201dc38(data_01ff8f64[gEventTextWindowPos]);
    EventScript_StartScreenSlideOut(index);
}
