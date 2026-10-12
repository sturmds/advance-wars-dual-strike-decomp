#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea* data_022aced4;

/* Moves a file's current block on for the next send. It steps forward by one
 * while at most 2 past nextBlock, and otherwise goes to nextBlock. */
void MbParentFile_AdvanceBlock(int fileIndex) {
  if (!MbComm_GetFile(data_022aced4, fileIndex)->active) {
    return;
  }
  if (MbComm_GetFile(data_022aced4, fileIndex)->pollBitmap == 0) {
    return;
  }
  if (MbComm_GetFile(data_022aced4, fileIndex)->nextBlock <=
      MbComm_GetFile(data_022aced4, fileIndex)->currentBlock) {
    if (MbComm_GetFile(data_022aced4, fileIndex)->currentBlock <=
        MbComm_GetFile(data_022aced4, fileIndex)->nextBlock + 2) {
      MbComm_GetFile(data_022aced4, fileIndex)->currentBlock++;
      return;
    }
  }
  MbComm_GetFile(data_022aced4, fileIndex)->currentBlock =
      MbComm_GetFile(data_022aced4, fileIndex)->nextBlock;
}
