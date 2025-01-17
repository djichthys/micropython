# Source: https://github.com/python/pyperformance
# License: MIT

# Calculating some of the digits of π.
# This benchmark stresses big integer arithmetic.
# Adapted from code on: http://benchmarksgame.alioth.debian.org/


def compose(a, b):
    aq, ar, as_, at = a
    bq, br, bs, bt = b
    return (aq * bq, aq * br + ar * bt, as_ * bq + at * bs, as_ * br + at * bt)


def extract(z, j):
    q, r, s, t = z
    return (q * j + r) // (s * j + t)


def gen_pi_digits(n):
    z = (1, 0, 0, 1)
    k = 1
    digs = []
    for _ in range(n):
        y = extract(z, 3)
        while y != extract(z, 4):
            z = compose(z, (k, 4 * k + 2, 0, 2 * k + 1))
            k += 1
            y = extract(z, 3)
        z = compose((10, -10 * y, 0, 1), z)
        digs.append(y)
    return digs

def run_pi_digits(iter, digits):
    for _ in range(iter):
      state =  gen_pi_digits(digits)

###########################################################################
# Benchmark interface
# executed benchmark param : (50, 25): (1, 35),
# execute: run_pi_digits(1,35)
