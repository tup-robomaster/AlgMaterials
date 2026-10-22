# 4-23 最佳秩 1 近似
# 取最大奇异值 σ1 和对应的单位向量 u1、v1，近似矩阵是 σ1·u1·v1ᵀ。
import math
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


data = sys.stdin.read().split()
it = iter(data)
a = float(next(it))
b = float(next(it))
c = float(next(it))
d = float(next(it))

p = a * a + c * c
q = a * b + c * d
r = b * b + d * d
t = p + r
det = p * r - q * q
disc = math.sqrt(max(t * t - 4 * det, 0.0))
l1 = (t + disc) / 2
s1 = math.sqrt(max(l1, 0.0))

if s1 == 0:
    R = [[0.0, 0.0], [0.0, 0.0]]
else:
    if abs(q) > 1e-12:
        v = [q, l1 - p]
    elif p >= r:
        v = [1.0, 0.0]
    else:
        v = [0.0, 1.0]
    n = math.sqrt(v[0] * v[0] + v[1] * v[1])
    v = [v[0] / n, v[1] / n]
    w = [a * v[0] + b * v[1], c * v[0] + d * v[1]]
    u = [w[0] / s1, w[1] / s1]
    R = [[s1 * u[i] * v[j] for j in range(2)] for i in range(2)]

print('[' + ', '.join('[' + ', '.join(f(x) for x in row) + ']'
                      for row in R) + ']')
