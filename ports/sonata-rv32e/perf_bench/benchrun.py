


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


arg_n, arg_m = 
bm_run(arg_n, arg_m)
