#!/usr/bin/env python3
"""Run mwldarm on build/objects.txt from a short working directory.

usage: link.py LD OUT LCF OBJECTS [LDFLAGS...]

mwldarm 2.0-72 compares each object's full path (current directory plus
the path given) with every object loaded before it, so the link costs
about objects^2 x the length of the paths' common prefix. With 5,073
objects in a worktree (common prefix about 105 characters) that was 11 s
of an 11.5 s link under load. This script makes a directory /tmp/awlXXXXXXXX,
puts one symlink in it per top-level directory under build/ (named 0, 1,
...), and runs the linker there with paths like 0/_dsd_gap@main_1.o. The
common prefix falls to about 33 characters and the link to about 4 s. The
output does not change: the LCF names objects by file name, and the ELF and
every .bin are byte-identical (docs/tool-issues.md).
"""

import os
import subprocess
import sys
import tempfile
from pathlib import Path


def main() -> int:
    ld, out, lcf, objects, *flags = sys.argv[1:]
    build = Path("build").resolve()
    paths = [line.strip().strip('"') for line in Path(objects).read_text().splitlines()]
    paths = [p for p in paths if p]
    tops: dict[str, str] = {}
    short = []
    for p in paths:
        parts = Path(p).parts
        if len(parts) < 3 or parts[0] != "build":
            print(f"link.py: object outside build/: {p}", file=sys.stderr)
            return 1
        alias = tops.setdefault(parts[1], str(len(tops)))
        short.append("/".join((alias, *parts[2:])))
    rsp = Path(out + ".rsp")
    rsp.write_text("\n".join(short) + "\n")

    tmp = Path(tempfile.mkdtemp(prefix="awl", dir="/tmp" if os.path.isdir("/tmp") else None))
    links = []
    try:
        for top, alias in tops.items():
            link = tmp / alias
            link.symlink_to(build / top)
            links.append(link)
        cmd = [str(Path(ld).resolve()), *flags, "-o", str(Path(out).resolve()),
               str(Path(lcf).resolve()), f"@{rsp.resolve()}"]
        return subprocess.run(cmd, cwd=tmp).returncode
    finally:
        for link in links:
            link.unlink()
        tmp.rmdir()


if __name__ == "__main__":
    sys.exit(main())
