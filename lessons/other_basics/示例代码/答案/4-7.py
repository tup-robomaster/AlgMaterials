# 4-7 求矩阵的秩
# 用 Fraction 做高斯消元，主元的个数就是秩。
import sys
from fractions import Fraction


def rref(M):
    A = [[Fraction(x) for x in row] for row in M]
    rows, cols = len(A), len(A[0])
    pivots = []
    r = 0
    for c in range(cols):
        p = None
        for i in range(r, rows):
            if A[i][c] != 0:
                p = i
                break
        if p is None:
            continue
        A[r], A[p] = A[p], A[r]
        v = A[r][c]
        A[r] = [x / v for x in A[r]]
        for i in range(rows):
            if i != r and A[i][c] != 0:
                k = A[i][c]
                A[i] = [x - k * y for x, y in zip(A[i], A[r])]
        pivots.append(c)
        r += 1
        if r == rows:
            break
    return A, pivots


data = sys.stdin.read().split()
it = iter(data)
m = int(next(it))
n = int(next(it))
M = [[Fraction(next(it)) for _ in range(n)] for _ in range(m)]

_, pivots = rref(M)
print('秩 = %d' % len(pivots))
