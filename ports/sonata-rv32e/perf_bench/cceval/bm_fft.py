# Copyright (c) 2019 Project Nayuki. (MIT License)
# https://www.nayuki.io/page/free-small-fft-in-multiple-languages

import math, cmath

signal = [math.cos(2 * math.pi * i / 128) + 0j for i in range(128)]
fft = None
fft_inv = None


def transform_radix2(vector, inverse):
    # Returns the integer whose value is the reverse of the lowest 'bits' bits of the integer 'x'.
    def reverse(x, bits):
        y = 0
        for i in range(bits):
            y = (y << 1) | (x & 1)
            x >>= 1
        return y
    # Initialization
    n = len(vector)
    levels = int(math.log(n) / math.log(2))
    coef = (2 if inverse else -2) * cmath.pi / n
    exptable = [cmath.rect(1, i * coef) for i in range(n // 2)]
    vector = [vector[reverse(i, levels)] for i in range(n)]  # Copy with bit-reversed permutation
    # Radix-2 decimation-in-time FFT
    size = 2
    while size <= n:
        halfsize = size // 2
        tablestep = n // size
        for i in range(0, n, size):
            k = 0
            for j in range(i, i + halfsize):
                temp = vector[j + halfsize] * exptable[k]
                vector[j + halfsize] = vector[j] - temp
                vector[j] += temp
                k += tablestep
        size *= 2
    return vector

def fft_run(iterations):
    for _ in range(iterations):
        fft = transform_radix2(signal, False)
        fft_inv = transform_radix2(fft, True)

###########################################################################
# executed params (50, 25): (2, 128),
# executed benchmark : fft_run(2)

