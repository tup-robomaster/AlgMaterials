# 4-17 求较大特征值与单位特征向量
# 对称矩阵的两个特征值都是实数；特征向量取第一个非零分量为正。
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

t = a + d
det = a * d - b * c
s = math.sqrt(max(t * t - 4 * det, 0.0))
lam = (t + s) / 2

if abs(b) > 1e-12:
    v = [b, lam - a]
elif abs(c) > 1e-12:
    v = [lam - d, c]
elif abs(a - lam) < 1e-12:
    v = [1.0, 0.0]
else:
    v = [0.0, 1.0]
n = math.sqrt(v[0] * v[0] + v[1] * v[1])
v = [v[0] / n, v[1] / n]
for x in v:
    if abs(x) > 1e-12:
        if x < 0:
            v = [-v[0], -v[1]]
        break

print('较大特征值 = ' + f(lam))
print('单位特征向量 = [' + f(v[0]) + ', ' + f(v[1]) + ']')
