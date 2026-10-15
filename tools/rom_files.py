#!/usr/bin/env python3
"""The ROM's file system as the build puts it into build/AWRE.nds.

    rom_files.py tree LIST [--complete]   build/rom/files/ and build/rom/path_order.txt, with the files LIST names
                                          left to the build; --complete: stop if one of those is missing
    rom_files.py romconfig     point build/build/rom_config.yaml at them

`dsd rom build` reads the file system from the directory `files_dir` names
and assigns file ids in the order of the File Name Table (each directory's
files by name, case ignored, then its subdirectories). Without this step the
build took extract/files/ as it is (issue 0050).

`tree` makes build/rom/files/ a copy of extract/files/ made of symbolic
links, except the paths in LIST (one per line, relative to files/), which
other build steps write there: tools/gfx.py packs each picture from its PNG
(docs/gfx.md, "Build") and tools/sound.py the sound archive from its files
(docs/sound.md, "Build"). A path of LIST that the ROM does not have is a new
file. A link or file that is in neither is removed, and so is a directory
left empty that extract/files/ does not have, since dsd would put an empty
directory into the ROM.

`dsd rom build` places the files' data in the order of path_order.txt
(extract/path_order.txt, which `dsd rom extract` writes from the ROM) and
leaves out a file no line covers: its FAT entry stays 0-0. build/rom/
path_order.txt adds a last line `/`, the root directory, which places every
file not placed yet after the rest. For the ROM's own files it changes
nothing, since the extracted list covers all of them.
"""

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXTRACT_FILES = Path("extract/files")
EXTRACT_ORDER = Path("extract/path_order.txt")
OUT = Path("build/rom")
FILES = OUT / "files"
PATH_ORDER = OUT / "path_order.txt"
STAMP = OUT / "files.stamp"
BUILT_LIST = OUT / "built.txt"  # the paths build steps write; tools/configure.py writes it
ROM_CONFIG = Path("build/build/rom_config.yaml")


def link_files(src_root: Path, out_root: Path, built: set[str]) -> tuple[int, int]:
    """Links in out_root to every file of src_root not in built; removes the
    rest. Returns (links made or kept, entries removed)."""
    wanted = {p.relative_to(src_root).as_posix() for p in src_root.rglob("*") if p.is_file()} - built
    removed = 0
    if out_root.exists():
        for p in sorted(out_root.rglob("*"), reverse=True):  # children before their directory
            rel = p.relative_to(out_root).as_posix()
            if p.is_symlink() or p.is_file():
                if rel in built and not p.is_symlink():
                    continue  # written by its build step
                if rel in wanted and p.is_symlink() and p.resolve() == (src_root / rel).resolve():
                    continue
                p.unlink()
                removed += 1
            elif p.is_dir() and not any(p.iterdir()) and not (src_root / rel).is_dir():
                p.rmdir()
                removed += 1
    for rel in sorted(wanted):
        link = out_root / rel
        if link.is_symlink():
            continue
        link.parent.mkdir(parents=True, exist_ok=True)
        link.symlink_to(os.path.relpath(src_root / rel, link.parent))
    return len(wanted), removed


def write_if_changed(path: Path, data: str | bytes) -> bool:
    """Write only a changed file, so ninja (restat) reruns only what changed.
    A symbolic link at path is replaced, not written through: build/rom/files/
    links to extract/files/. Used by the steps that write files there
    (tools/gfx.py, tools/sound.py)."""
    if isinstance(data, str):
        data = data.encode()
    if path.is_file() and not path.is_symlink() and path.read_bytes() == data:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.is_symlink():
        path.unlink()
    path.write_bytes(data)
    return True


def cmd_tree(args: list[str]) -> int:
    if len(args) not in (1, 2) or args[1:] not in ([], ["--complete"]):
        sys.exit(__doc__)
    if not (ROOT / EXTRACT_FILES).is_dir():
        sys.exit(f"error: {EXTRACT_FILES} is missing; run make extract")
    built = {line.strip() for line in (ROOT / args[0]).read_text().splitlines() if line.strip()}
    linked, removed = link_files(ROOT / EXTRACT_FILES, ROOT / FILES, built)
    order = (ROOT / EXTRACT_ORDER).read_text().splitlines()
    write_if_changed(ROOT / PATH_ORDER, "\n".join([*order, "/"]) + "\n")
    # Rewritten only when the counts change, which a change of LIST or of
    # extract/files/ does: the ROM BUILD step runs tree too, and must not
    # put its own input out of date.
    write_if_changed(ROOT / STAMP, f"{linked} files from {EXTRACT_FILES}, {len(built)} from build steps\n")
    if removed:
        print(f"rom_files: removed {removed} old entries from {FILES}")
    # A built path that held a link when ninja planned the build (a YAML file
    # put back with an old date) lost it above, after its step was judged up
    # to date. Stop rather than build a ROM without the file; the next run
    # sees the file missing and writes it.
    missing = sorted(rel for rel in built if not (ROOT / FILES / rel).is_file())
    if missing and "--complete" in args:
        sys.exit(f"rom_files: {len(missing)} files of the build steps are missing from {FILES}, "
                 f"such as {missing[0]}; run make again")
    return 0


def cmd_romconfig(args: list[str]) -> int:
    """dsd rom config writes the extract/ paths; the paths are relative to the
    rom_config.yaml file."""
    path = ROOT / ROM_CONFIG
    text = path.read_text()
    for key, target in (("files_dir", FILES), ("path_order", PATH_ORDER)):
        rel = os.path.relpath(ROOT / target, path.parent)
        text, n = re.subn(rf"^{key}: .*$", f"{key}: {rel}", text, flags=re.M)
        if n != 1:
            sys.exit(f"rom_files.py: no {key} line in {ROM_CONFIG}")
    path.write_text(text)
    return 0


def main() -> int:
    cmds = {"tree": cmd_tree, "romconfig": cmd_romconfig}
    if len(sys.argv) < 2 or sys.argv[1] not in cmds:
        sys.exit(__doc__)
    return cmds[sys.argv[1]](sys.argv[2:])


if __name__ == "__main__":
    sys.exit(main())
