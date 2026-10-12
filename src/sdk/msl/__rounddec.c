#include "types.h"
#include "sdk/msl.h"

void __rounddec(Decimal* dec, int count) {
  int roundUp;

  if (count <= 0) {
    return;
  }
  if (count >= dec->length) {
    return;
  }
  roundUp = __must_round(dec, count);
  dec->length = count;
  if (roundUp < 0) {
    return;
  }
  __dorounddecup(dec, count);
}
