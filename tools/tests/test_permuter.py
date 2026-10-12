#!/usr/bin/env python3
"""Check the parts of tools/permuter.py that need no ROM and no decomp-permuter.

    python3 tools/tests/test_permuter.py

1. canon_line rewrites a relocation to a known symbol as its address plus
   the addend, so two names for one address compare equal, and leaves
   unknown symbols and other lines alone.
2. The objdump script written for decomp-permuter compiles.
3. parked_names reads the first column of the parked table.
4. function_text finds one definition, braces nested inside it included.
5. parser_safe hides an `asm` declaration from decomp-permuter's C parser,
   keeps the line count, and the parser accepts the result and prints the
   declaration back unchanged (issue 0068). Skipped when decomp-permuter is
   not in .cache/ (make setup fetches it).
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import permuter  # noqa: E402

failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def main() -> int:
    addrs = {"data_ov000_022dbd7c": 0x022dbd7c, "Ov000_gMapHeaders": 0x022dbd28}
    a = permuter.canon_line("\t\t\tb54: R_ARM_ABS32\tdata_ov000_022dbd7c", addrs)
    b = permuter.canon_line("\t\t\tb54: R_ARM_ABS32\tOv000_gMapHeaders+0x54", addrs)
    check("two names for one address agree", a == b and a.endswith("0x022dbd7c"), f"{a!r} {b!r}")
    c = permuter.canon_line("\t\t\t10: R_ARM_PC24\tOv000_gMapHeaders-0x8", addrs)
    check("negative addend", c.endswith("0x022dbd20"), c)
    for line in ("\t\t\t20: R_ARM_PC24\tSomething_Unknown", "  14:\te1a00000 \tmov\tr0, r0"):
        check(f"left alone: {line.split()[-1]}", permuter.canon_line(line, addrs) == line)

    script = permuter.OBJDUMP.format(canon=permuter.inspect.getsource(permuter.canon_line))
    try:
        compile(script, "objdump.py", "exec")
        check("objdump script compiles", True)
    except SyntaxError as e:
        check("objdump script compiles", False, str(e))

    table = """# Parked
| function | module:address | file |
|---|---|---|
| RangeMap_MarkAdjacentTiles | itcm:01ffaa3c | `x.c` |
| func_ov002_02358c40 | ov002:02358c40 | `y.c` |
Not a row | here |
"""
    with tempfile.TemporaryDirectory() as tmp:
        p = Path(tmp) / "parked.md"
        p.write_text(table)
        names = permuter.parked_names(p)
    check("parked names", names == ["RangeMap_MarkAdjacentTiles", "func_ov002_02358c40"], str(names))

    src = "int Other(void);\nint Foo(int a)\n{\n    if (a) {\n        return 1;\n    }\n    return 0;\n}\nint Bar(void) { return 2; }\n"
    body = permuter.function_text(src, "Foo")
    check("function text", body.startswith("int Foo(int a)") and body.endswith("return 0;\n}"), repr(body))
    check("declaration is not a definition", permuter.function_text(src, "Other") == "")

    base = "typedef unsigned int u32;\nasm void _ll_udiv_common(void);\n  asm void _ll_div_core(void);\n" \
           "u32 Div(u32 a, u32 b)\n{\n    return a / b;\n}\n"
    safe = permuter.parser_safe(base)
    check("asm lines hidden", "asm" not in safe and safe.count("#pragma _permuter b64literal") == 2, safe)
    check("line count kept", safe.count("\n") == base.count("\n"))
    check("other lines kept", safe.splitlines()[0] == "typedef unsigned int u32;" and safe.endswith("return a / b;\n}\n"))
    if permuter.PERMUTER.exists():
        with tempfile.TemporaryDirectory() as tmp:
            permuter.WORK = Path(tmp)
            (permuter.WORK / "_shim").mkdir()
            (permuter.WORK / "_shim" / "toml.py").write_text(permuter.SHIM)
            raw = permuter.parse_error(base)
            check("parser rejects the raw asm declaration", raw is not None and "Syntax error" in raw, str(raw))
            check("parser accepts the hidden form", permuter.parse_error(safe) is None, str(permuter.parse_error(safe)))
            from src import ast_util  # noqa: E402
            printed = ast_util.to_c(ast_util.parse_c(safe))
            check("printed source has the asm declarations back",
                  "asm void _ll_udiv_common(void);" in printed and "asm void _ll_div_core(void);" in printed, printed)
    else:
        print("parser checks: skipped, decomp-permuter is missing (make setup)")

    if failures:
        print(f"test_permuter: {len(failures)} failed", file=sys.stderr)
        return 1
    print("test_permuter: ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
