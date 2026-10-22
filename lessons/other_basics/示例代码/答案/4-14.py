# 4-14 旋转后平移
# 先算 R p，再加上平移量 (tx, ty)。
import math
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


data = sys.stdin.read().split()
it = iter(data)
deg = float(next(it))
tx = float(next(it))
ty = float(next(it))
x = float(next(it))
y = float(next(it))

t = math.radians(deg)
c, s = math.cos(t), math.sin(t)
rx = c * x - s * y + tx
ry = s * x + c * y + ty
print('[' + f(rx) + ', ' + f(ry) + ']')
