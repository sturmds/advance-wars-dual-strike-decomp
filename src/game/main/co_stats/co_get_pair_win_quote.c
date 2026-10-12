#include "types.h"
#include "game/co_data.h"

const CoTagRecord* Co_GetTagRecord(int co, int partner);
u32 Co_PickWinQuote(int co);

/* Win quote text id for line speaker (0: co, 1: partner) of a tag pair. pick
 * (0 or 1) chooses between the two quote sets of the pair's tag record, laid
 * out as CoTagRecord's first four TextRefs. Without a partner, or without a
 * record, one of the speaker's own quotes. */
u32 Co_GetPairWinQuote(int co, int partner, int speaker, int pick) {
  const CoTagRecord* record;

  if (partner == 0) {
    return Co_PickWinQuote(co);
  }
  record = Co_GetTagRecord(co, partner);
  if (record == NULL) {
    if (speaker != 0) {
      co = partner;
    }
    return Co_PickWinQuote(co);
  }
  return (&record->winQuote1)[speaker + (pick % 2) * 2].id;
}
