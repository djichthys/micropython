# This tests qstr_find_strn() speed when the string being searched for is not found.


def test(r):
    for _ in r:
        str("a string that shouldn't be interned")


###########################################################################
# Benchmark interface
# bm_param chosen : (32, 10): (400,),
# execute : test(range(400))

