# Save files

`tools/savedit.py` reads and changes a save file (`.sav`) field by field, with the layouts of our headers. melonDS keeps the save as a raw copy of the cartridge's 256 KB backup chip, which is the format the tool reads; flash carts usually do the same. A save from another emulator may carry a footer after the 0x40000 bytes; the tool ignores anything past the slots.

## Layout

The game splits the chip into `SAVE_SLOT_COUNT` (21) slots of `sizeof(SaveBuffer)` (0x3000) bytes; slot *i* starts at `i * 0x3000` (`Save_ReadFromBackup`). Each slot is one `SaveBuffer` (`include/game/save_data.h`): magic `"dswj"`, `version` 29, a byte sum and its inverse, a save counter, the record id, the part number, and up to 0x2f99 bytes of record data with its offset in the record. `Save_CheckBuffer` fails a slot whose magic, version, end mark or sum is wrong.

A record holds what one call of `Save_WriteRecord` writes, split over one or more slots. Writes go to free slots, so a slot that held an older copy is still valid on the chip after the record moves. The first part of the newest record 0 lists in `slotIds` which record each slot holds now; `Save_LoadRecord` and `savedit.py` go by that list.

| Record | Struct | What |
|---|---|---|
| 0 | `SaveProgressBlocks` (0x1684 bytes) | Progress: game flags, `PlayRecords` (CO experience and unlocks, War Room and campaign records, counters), the three campaign profiles, Survival and link data. `Save_PackProgress` copies the live data into it unchanged. |
| others | `BattleSaveRecord`, `DesignMapRecord` | Battle saves and Design Room maps. Their record ids are not mapped yet; pass `--record N --type STRUCT`. |

## The tool

```sh
python3 tools/savedit.py slots SAV
python3 tools/savedit.py show SAV 'playRecords.coRecords[CO_SONJA]'
python3 tools/savedit.py set SAV PATH=VALUE ... -o OUT
```

Paths start inside the record's struct. Indexes and values take numbers, enum and `#define` names from `include/`, and sums of those. The layouts come from `build/dsindex.json` (`tools/dsindex.py`, compiled from our headers), so a field that is added, named or moved in a header can be used right away. When a header change makes the struct's size differ from the record's length, the tool stops instead of guessing.

`set` writes the new values into the slots that hold the record and fixes their checksums. It leaves the counters and slot ids alone, so a record keeps its length.

What the headers do not say has to come from reading the code:

- Rank is experience / 1000, at most 100 (`ExperienceToRank`). `Co_SetExperience` caps experience at 100000; the field has room for more.
- A War Room score of 280 or more is S rank; 250 and 200 start the next two ranks (`Score_ToRank_020e5428`). A record with `days` 0 counts as empty.
- War Room tables (`warRoomRecords[table]`, the map select's sub strip): 0 one CO with skills off, 1 one CO with skills, 2 tag with skills off, 3 tag with skills (`Skills_AreDisabled`, `SetPlayer1TagPartner`). Maps are indexed from `WAR_ROOM_FIRST_MAP` (0x8b, Spann Island).
- `coRecords[co].available` decides whether a CO shows in the CO lists (`Co_IsAvailable`).
- Totals such as `playRecords.coRankTotal` are not recalculated. The game recalculates that one when it next awards experience (`PlayRecords_UpdateCoRankTotal`).

## Example

Sonja at rank 7 and unlocked, and an S rank on Spann Island with Kanbei in the one-CO, no-skills table:

```sh
W='playRecords.warRoomRecords[0][0x8b - WAR_ROOM_FIRST_MAP][0]'
python3 tools/savedit.py set build/AWRE.sav \
    'playRecords.coRecords[CO_SONJA].experience=7000' \
    'playRecords.coRecords[CO_SONJA].available=1' \
    "$W.frontCo=CO_KANBEI" "$W.days=7" "$W.score=300" -o build/edited.sav
python3 tools/emu.py play --save build/edited.sav
```

`emu.py play --save` starts melonDS on a copy of the file. To use the save in your own melonDS, put it next to the ROM with the ROM's name and `.sav` (the default when its save path setting is empty), or use File → Import savefile.
