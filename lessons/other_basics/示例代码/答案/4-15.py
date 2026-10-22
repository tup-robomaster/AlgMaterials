# 4-15 复合两个变换
# 每个变换用齐次矩阵表示；先做 T1 再做 T2，合起来是 T2·T1。
import math
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def trans(deg, tx, ty):
    t = math.radians(deg)
    c, s = math.cos(t), math.sin(t)
    return [[c, -s, tx], [s, c, ty], [0.0, 0.0, 1.0]]


def mul(A, B):
    return [[sum(A[i][k] * B[k][j] for k in range(3)) for j in range(3)]
            for i in range(3)]


def apply(T, p):
    return [T[0][0] * p[0] + T[0][1] * p[1] + T[0][2],
            T[1][0] * p[0] + T[1][1] * p[1] + T[1][2]]


data = sys.stdin.read().split()
it = iter(data)
d1 = float(next(it))
tx1 = float(next(it))
ty1 = float(next(it))
d2 = float(next(it))
tx2 = float(next(it))
ty2 = float(next(it))
x = float(next(it))
y = float(next(it))

T1 = trans(d1, tx1, ty1)
T2 = trans(d2, tx2, ty2)
q = apply(mul(T2, T1), [x, y])
print('[' + f(q[0]) + ', ' + f(q[1]) + ']')
