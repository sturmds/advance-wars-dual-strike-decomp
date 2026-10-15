#!/usr/bin/env python3
"""Check tools/rom_files.py.

    python3 tools/tests/test_rom_files.py

- link_files: on a throwaway tree, links every source file except the built
  ones; keeps a built file; replaces a link at a path that becomes built;
  removes links and files of paths that left both sets, and a directory
  left empty that the source does not have; a second run changes nothing.
- With a build present (build/rom/): the tree links every file of
  extract/files/ that the build does not pack, each to its own source, and
  path_order.txt is the extracted list plus `/`. Skipped with a message
  otherwise.
"""

import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import rom_files  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got!r}\n  want: {want!r}")
        failures.append(what)


def listing(root: Path) -> dict[str, str]:
    """Each entry under root: 'dir', 'file:<bytes>' or 'link:<target bytes>'."""
    out = {}
    for p in sorted(root.rglob("*")):
        rel = p.relative_to(root).as_posix()
        out[rel] = "link:" + p.read_text() if p.is_symlink() else "dir" if p.is_dir() else "file:" + p.read_text()
    return out


with tempfile.TemporaryDirectory() as tmp:
    src, out = Path(tmp) / "src", Path(tmp) / "out"
    for rel in ("a/1", "a/2", "b/3"):
        (src / rel).parent.mkdir(parents=True, exist_ok=True)
        (src / rel).write_text(rel)
    rom_files.link_files(src, out, set())
    expect("every file linked", listing(out),
           {"a": "dir", "a/1": "link:a/1", "a/2": "link:a/2", "b": "dir", "b/3": "link:b/3"})
    expect("links are relative", os.readlink(out / "a/1"), "../../src/a/1")

    # a/2 becomes built (its pack step writes it after the tree), new/4 is a new file.
    rom_files.link_files(src, out, {"a/2", "new/4"})
    expect("a link at a built path is removed", (out / "a/2").exists(), False)
    (out / "a/2").write_text("packed")
    (out / "new").mkdir()
    (out / "new/4").write_text("added")
    rom_files.link_files(src, out, {"a/2", "new/4"})
    expect("built files stay", (listing(out)["a/2"], listing(out)["new/4"]), ("file:packed", "file:added"))

    # Both leave the build: a/2 goes back to its link, new/ goes away.
    (out / "extra").write_text("x")
    _, removed = rom_files.link_files(src, out, set())
    expect("files back to links, new and extra entries removed", listing(out),
           {"a": "dir", "a/1": "link:a/1", "a/2": "link:a/2", "b": "dir", "b/3": "link:b/3"})
    expect("entries removed", removed, 4)  # a/2, new/4, new/, extra
    expect("a second run removes nothing", rom_files.link_files(src, out, set()), (3, 0))

    (src / "empty").mkdir()
    (out / "empty").mkdir()
    rom_files.link_files(src, out, set())
    expect("an empty directory of the source stays", (out / "empty").is_dir(), True)

tree = ROOT / rom_files.FILES
if (ROOT / rom_files.STAMP).exists() and (ROOT / rom_files.EXTRACT_FILES).is_dir():
    built = set((ROOT / rom_files.BUILT_LIST).read_text().split())
    bad = []
    for p in sorted((ROOT / rom_files.EXTRACT_FILES).rglob("*")):
        rel = p.relative_to(ROOT / rom_files.EXTRACT_FILES).as_posix()
        if p.is_file() and rel not in built and (tree / rel).resolve() != p.resolve():
            bad.append(rel)
    expect("build/rom/files links every file the build does not pack", bad, [])
    order = (ROOT / rom_files.EXTRACT_ORDER).read_text().splitlines()
    expect("path_order.txt ends in /", (ROOT / rom_files.PATH_ORDER).read_text().splitlines(), [*order, "/"])
else:
    print("build/rom/files: skipped, no build (run make)")

if failures:
    sys.exit(f"test_rom_files: {len(failures)} failed")
