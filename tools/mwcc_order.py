"""The order in which mwccarm 2.0 emits the data objects of one file.

Each data or .bss object goes into a section of its own, and the compiler
heapsorts them by size before writing them. The sort starts from the
uninitialized objects (plain or `static`) in definition order, followed by
the initialized ones, zero-initialized included, in reverse definition
order. Objects of equal size come out in an order that depends only on
their positions. Function-scope statics follow another order and are not
modelled. tools/tests/test_data_order.py and tools/tests/test_bss.py check
the model against the compiler.

Used by tools/maps.py (the unit lists in ov000) and tools/bss.py.
"""


def mwcc_heapsort(sizes: list[int], initialized: list[bool] | None = None) -> list[int]:
    """Takes the sizes in definition order, and for each object whether it
    has an initializer (all of them when not given). Returns the definition
    indices in the order the compiler emits them."""
    if initialized is None:
        initialized = [True] * len(sizes)
    a = [i for i in range(len(sizes)) if not initialized[i]]
    a += [i for i in range(len(sizes)) if initialized[i]][::-1]

    def sift(root: int, end: int) -> None:
        while 2 * root + 1 <= end:
            child = 2 * root + 1
            top = root if sizes[a[root]] >= sizes[a[child]] else child
            if child + 1 <= end and sizes[a[top]] < sizes[a[child + 1]]:
                top = child + 1
            if top == root:
                return
            a[root], a[top] = a[top], a[root]
            root = top

    for start in range((len(a) - 2) // 2, -1, -1):
        sift(start, len(a) - 1)
    for end in range(len(a) - 1, 0, -1):
        a[0], a[end] = a[end], a[0]
        sift(0, end - 1)
    return a


def definition_order(sizes: list[int], initialized: bool = True) -> list[int]:
    """The definition order that mwccarm emits as 0, 1, 2, ...: indices into
    `sizes`, which must not decrease. All objects are initialized, or none.
    Given the sizes, mwcc_heapsort's permutation depends only on positions,
    so its inverse on the sorted sizes is the answer. Raises ValueError when
    the sizes decrease somewhere: no definition order gives that order."""
    if sizes != sorted(sizes):
        raise ValueError("the sizes are not in ascending order")
    flags = [initialized] * len(sizes)
    emitted = mwcc_heapsort(sizes, flags)
    order = [0] * len(sizes)
    for pos, defined in enumerate(emitted):
        order[defined] = pos
    if [order[i] for i in mwcc_heapsort([sizes[i] for i in order], flags)] != list(range(len(sizes))):
        raise AssertionError("definition_order does not invert mwcc_heapsort")
    return order
