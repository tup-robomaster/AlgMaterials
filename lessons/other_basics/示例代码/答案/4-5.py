# 4-5 3×3 行列式
# 按第一行展开，三个 2×2 行列式用对角线法则。
import sys
from fractions import Fraction


def f(x):
    s = '%.3f' % float(x)
    return '0.000' if s == '-0.000' else s


def num(q):
    if q.denominator == 1:
        return str(q.numerator)
    return f(q)


def det2(m):
    return m[0][0] * m[1][1] - m[0][1] * m[1][0]


def det3(M):
    return (M[0][0] * det2([[M[1][1], M[1][2]], [M[2][1], M[2][2]]])
            - M[0][1] * det2([[M[1][0], M[1][2]], [M[2][0], M[2][2]]])
            + M[0][2] * det2([[M[1][0], M[1][1]], [M[2][0], M[2][1]]]))


data = sys.stdin.read().split()
it = iter(data)
M = [[Fraction(next(it)) for _ in range(3)] for _ in range(3)]

print('行列式 = ' + num(det3(M)))
