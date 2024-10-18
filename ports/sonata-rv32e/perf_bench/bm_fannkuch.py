# Source: https://github.com/python/pyperformance
# License: MIT

# The Computer Language Benchmarks Game
# http://benchmarksgame.alioth.debian.org/
# Contributed by Sokolov Yura, modified by Tupteq.


def fannkuch(n):
    count = list(range(1, n + 1))
    max_flips = 0
    m = n - 1
    r = n
    check = 0
    perm1 = list(range(n))
    perm = list(range(n))
    perm1_ins = perm1.insert
    perm1_pop = perm1.pop

    while 1:
        if check < 30:
            check += 1

        while r != 1:
            count[r - 1] = r
            r -= 1

        if perm1[0] != 0 and perm1[m] != m:
            perm = perm1[:]
            flips_count = 0
            k = perm[0]
            while k:
                perm[: k + 1] = perm[k::-1]
                flips_count += 1
                k = perm[0]

            if flips_count > max_flips:
                max_flips = flips_count

        while r != n:
            perm1_ins(r, perm1_pop(0))
            count[r] -= 1
            if count[r] > 0:
                break
            r += 1
        else:
            return max_flips


###########################################################################
# Benchmark interface

bm_params = {
    (32, 10): (3,),
    (50, 10): (5,),
    (100, 10): (6,),
    (500, 10): (7,),
    (1000, 10): (8,),
    (5000, 10): (9,),
}


def bm_setup(params):
    state = None

    def run():
        nonlocal state
        state = fannkuch(params[0])

    def result():
        return params[0], state

    return run, result



def bm_run(N, M):

    # Pick sensible parameters given N, M
    cur_nm = (0, 0)
    param = None
    for nm, p in bm_params.items():
        if 10 * nm[0] <= 12 * N and nm[1] <= M and nm > cur_nm:
            cur_nm = nm
            param = p
    if param is None:
        print(-1, -1, "SKIP: no matching params")
        return

    # Run and time benchmark
    run, result = bm_setup(param)
    run()
    norm, out = result()
    print(norm, out)


arg_n, arg_m = (500,10)
bm_run(arg_n, arg_m)
