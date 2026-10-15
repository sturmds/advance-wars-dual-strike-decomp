# Tenets

Short, numbered and editable. Every change, generated or hand-written, is checked against this list by the review pass (`.claude/skills/tenets-review/SKILL.md`).

1. A change that breaks `make release` is not done, whatever else it achieves. Every commit on `mainline` passes `make setup && make release` from a clean clone.
2. Match first, then make it readable. Never trade a match for style. Never stop at a match when the meaning is known.
3. No magic offsets once a struct is known. `actor->hp`, not `*(s16 *)(self + 0x2c)`.
4. Names describe behavior. Address names (`func_02012345`) are allowed only until the behavior is understood.
5. A comment explains why the code has an odd shape ("needed to match: the compiler keeps this temp in r4"), not what the next line does.
6. Write plainly in comments, docs and commit messages. No filler words, no hype. Write it the way you'd say it to a teammate.
7. Credit every outside source we take from: code, tools, names, facts or ideas. Each source has a page in `docs/third-party/` saying what we took and under which license, and the file or commit that uses it says so (a header comment for code and tools, an `rh890:`-style line in commit messages for facts). Never copy from a source whose license does not allow it. When a source states no license, take only facts (names, layouts, values checked against our ROM) and credit it.
8. Privacy comes before publishing anything. Nothing in a file, commit message or commit header identifies a person: no real names or names made from them, home directories or machine paths, account, session or private-document links, or names read from file metadata. Every contributor signs commits with their own public handle (the maintainer's is sturmds), and nothing may tie a handle to the person behind it. Paths are relative to the repo root.
9. Git holds only what we write. The ROM, the SDK, the compiler and the game's assets never enter git, including assets we have turned into a text form (maps, graphics, text, sound). `make setup` unpacks them from the builder's own ROM into `extracted/`. Files under `assets/` say how to read and rebuild the data (legends, layouts, order), never what it holds.
10. Decompiled, never leaked. Code and names come from our own reading of the ROM or from a credited public source (tenet 7), never from leaked SDK or game source, directly or through another project. A name we cannot trace to either gets a name of our own.
11. Readability: a reader finds each thing in one place, under one name. Every function, global and shared macro is declared once, in the header of its topic, and C files include headers instead of copying declarations. Neighbouring code of one topic shares one file. A copy or an odd form stays only when the match needs it, and says so. A readability change leaves every function's code as it was (`tools/objcmp.py compare`). The method is in `docs/readability.md`.
