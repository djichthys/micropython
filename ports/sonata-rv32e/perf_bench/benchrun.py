#!/usr/bin/python3 

bm_params = { 
  "bm_chaos": {
                (100, 50): (0.25, 100, 50, 50, 50, 1234),
                (1000, 1000): (0.25, 200, 400, 400, 1000, 1234),
                (5000, 1000): (0.25, 400, 500, 500, 7000, 1234),
              },
  "bm_fannkuch": {
                (32, 10): (3,),
                (50, 10): (5,),
                (100, 10): (6,),
                (500, 10): (7,),
                (1000, 10): (8,),
                (5000, 10): (9,),
              }, 
  "bm_fft": {
                (50, 25): (2, 128),
                (100, 100): (3, 256),
                (1000, 1000): (20, 512),
                (5000, 1000): (100, 512),
            }, 
  "bm_nqueens": 




              
}

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


if __name__ == '__main__': 
    for bm in bm_params.keys(): 
      pr
