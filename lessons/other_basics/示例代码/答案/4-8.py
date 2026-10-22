# 4-8 判断方程组解的情况
# 增广矩阵每行最后一个是等号右边的常数。
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
M = [[Fraction(next(it)) for _ in range(n + 1)] for _ in range(m)]

R, pivots = rref(M)
bad = False
for row in R:
    if all(x == 0 for x in row[:n]) and row[n] != 0:
        bad = True
if bad:
    print('无解')
elif len(pivots) < n:
    print('无穷多解')
else:
    print('唯一解')
