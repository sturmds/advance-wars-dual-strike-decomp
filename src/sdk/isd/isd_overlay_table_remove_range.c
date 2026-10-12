/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/fs.h"
#include "sdk/isd.h"

/*
 * Removes the range [address, address + size) from target's overlay table:
 * entries inside it go, an entry it starts in is split, an entry it ends in
 * is trimmed. Returns FALSE when target has no table.
 */
BOOL Isd_OverlayTableRemoveRange(MIProcessor target, u32 address, u32 size) {
  IsdOverlayInfo* table = gIsdOverlayTables[target];
  IsdRegionState saved;
  BOOL found;
  u64 end;
  u32 i;

  if (table == NULL) {
    return FALSE;
  }
  if (size == 0) {
    return TRUE;
  }
  Isd_OpenDebugRegion(&saved);
  while (Isd_SwapWord(ISD_OVERLAY_LOCKED, &table->header.lock) != 0) {
  }
  table->header.flags |= ISD_OVERLAY_FLAG_UPDATING;
  /* A range that runs past the top of the address space stops there. */
  if ((u64)address + size > 0x100000000ULL) {
    size = 0x100000000ULL - address;
  }
  found = FALSE;
  end = (u64)address + size;
  /*
     * Find the first entry the range overlaps: one that lies inside it (the range
     * then starts at that entry) or one that holds address.
     */
  for (i = 0; i < table->header.infoCount && !found; i++) {
    u32 vma = table->info[i].vma;

    if (vma > address && (u64)vma + table->info[i].size <= end) {
      size -= vma - address;
      address = vma;
      found = TRUE;
    }
    if (vma <= address && address < (u64)vma + table->info[i].size) {
      found = TRUE;
    }
    if (!found) {
      continue;
    }
    if (vma > address) {
      Isd_ReportFailure(gIsdOverlayFileName, 480, gIsdEntryStartCheck, TRUE);
    }
    if (!((u64)table->info[i].vma + table->info[i].size > address)) {
      Isd_ReportFailure(gIsdOverlayFileName, 481, gIsdEntryEndCheck, TRUE);
    }
    if (address != table->info[i].vma) {
      /* Split the entry at address: the first copy keeps the part below it. */
      Isd_OverlayTableInsert(table, i);
      table->info[i] = table->info[i + 1];
      table->info[i].size = address - table->info[i].vma;
      table->info[i + 1].vma += table->info[i].size;
      table->info[i + 1].fileOffset += table->info[i].size;
      table->info[i + 1].size -= table->info[i].size;
      i++;
    }
    if (address != table->info[i].vma) {
      Isd_ReportFailure(gIsdOverlayFileName, 494, gIsdEntryAtAddressCheck,
                        TRUE);
    }
    if (table->info[i].size <= size) {
      Isd_OverlayTableRemove(table, i);
      /* Remove the entries the range covers, and trim the one it ends in. */
      while (i < table->header.infoCount) {
        s64 wrapSize;

        if ((u64)table->info[i].vma + table->info[i].size <= end) {
          Isd_OverlayTableRemove(table, i);
          continue;
        }
        wrapSize = end - table->info[i].vma;
        if (wrapSize <= 0) {
          break;
        }
        if (!(table->info[i].size > (u32)wrapSize)) {
          Isd_ReportFailure(gIsdOverlayFileName, 513, gIsdWrapSizeCheck, TRUE);
        }
        table->info[i].vma += (u32)wrapSize;
        table->info[i].fileOffset += (u32)wrapSize;
        table->info[i].size -= (u32)wrapSize;
      }
    } else {
      table->info[i].vma += size;
      table->info[i].fileOffset += size;
      table->info[i].size -= size;
    }
  }
  table->header.flags &= ~ISD_OVERLAY_FLAG_UPDATING;
  table->header.lock = 0;
  Isd_CloseDebugRegion(&saved);
  return TRUE;
}
