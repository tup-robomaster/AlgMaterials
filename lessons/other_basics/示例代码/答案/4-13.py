# 4-13 旋转一个点
# 逆时针旋转 θ 的矩阵是 [[cosθ,-sinθ],[sinθ,cosθ]]。
import math
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


data = sys.stdin.read().split()
it = iter(data)
deg = float(next(it))
x = float(next(it))
y = float(next(it))

t = math.radians(deg)
c, s = math.cos(t), math.sin(t)
rx = c * x - s * y
ry = s * x + c * y
print('[' + f(rx) + ', ' + f(ry) + ']')
