# 4-12 3×3 求逆
# 把 A 和单位矩阵并排，做高斯—约当消元；左半边变成单位矩阵时，右半边就是逆。
import sys
from fractions import Fraction


def f(x):
    s = '%.3f' % float(x)
    return '0.000' if s == '-0.000' else s


def show_mat(M):
    return '[' + ', '.join(
        '[' + ', '.join(f(x) for x in row) + ']' for row in M) + ']'


def inverse(M):
    n = len(M)
    A = []
    for i in range(n):
        A.append([Fraction(M[i][j]) for j in range(n)] +
                 [Fraction(1 if i == j else 0) for j in range(n)])
    for c in range(n):
        p = None
        for i in range(c, n):
            if A[i][c] != 0:
                p = i
                break
        if p is None:
            return None
        A[c], A[p] = A[p], A[c]
        v = A[c][c]
        A[c] = [x / v for x in A[c]]
        for i in range(n):
            if i != c and A[i][c] != 0:
                k = A[i][c]
                A[i] = [x - k * y for x, y in zip(A[i], A[c])]
    return [row[n:] for row in A]


data = sys.stdin.read().split()
it = iter(data)
M = [[Fraction(next(it)) for _ in range(3)] for _ in range(3)]

inv = inverse(M)
if inv is None:
    print('不可逆')
else:
    print(show_mat(inv))
