# Tool issues

Problems found in third-party tools, with what we do about them. Kept local; nothing here has been reported upstream.

## dsd: `init` aborts on AWRE

- **Versions:** dsd 0.12.0 and 0.12.1 (`tools.lock`). The unreleased commits after v0.12.1 at `c408063` change the disassembler, not `init`'s analysis, so a source build is not expected to help (not tested: building it needs cargo).
- **Command:** `dsd init -r extract/config.yaml -o <out> -b build`
- **Error:**
  ```
  [ERROR] Local function call from 0x021347a6 in ARM9 main to 0x021341aa leads to no function
  ```
- **Cause:** after the last function in main's `.text` (ends 0x02129760), `init` keeps looking for function starts for up to 0x2000 bytes (`max_function_start_search_distance` in `lib/src/config/module.rs`). The bytes after `.text` are `.rodata`, starting with the MSL exception table and then graphics data. They decode as plausible Thumb code up to 0x02134968, and a `bl` in that fake code targets an address with no function, which is a hard error.
- **Workaround:** `tools/dsd_init.py` runs `init` on a temporary copy of the extract with 0x2100 bytes zeroed at 0x02129760, then points the config at the real extract and writes main's real code hash (`tools/fxhash.py`).
- **Cost of the workaround:** `init` records no pointers in 0x02129760–0x0212b860. The 29 real ones (exception table) were added to `config/AWRE/arm9/relocs.txt` by hand. Rerunning `dsd_init.py --force` drops them; restore them from git.
- **Possible upstream fix:** stop the search at the end of `.text` when the module's section boundaries are known, or turn "leads to no function" into a warning for addresses outside `.text`.

## dsd: `rom build` without an ARM7 BIOS

- **Behaviour:** without `--arm7-bios`, `dsd rom build` writes 0 for the secure-area CRC (header 0x6C), so the header CRC (0x15E) differs too. 4 bytes in total.
- **Workaround:** `tools/fix_header.py` checks that our secure area (0x4000–0x7FFF) equals the original's, copies the original secure-area CRC, and recomputes the header CRC. No BIOS dump needed.
- **Not a bug:** the CRC covers the encrypted secure area, and the encryption key comes from the BIOS.

## dsd: smaller warnings

- `init` warns 28 times about calls into the middle of functions, all in runtime and floating-point assembly in 0x0211e980–0x02129760. It adds labels for them; the build is unaffected.
- `dsd fix find-exceptix` reports "__FindExceptionTable not found". Not needed for M0.
- `dsd objdiff` names the top-level `target_dir` and `base_dir` the other way round from objdiff's convention. The per-unit `target_path` (original) and `base_path` (ours) are correct, and those are what objdiff uses.
