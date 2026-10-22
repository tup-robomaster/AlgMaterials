# 4-16 求 2×2 实特征值
# 特征方程 λ² - (a+d)λ + (ad-bc) = 0，判别式小于 0 时没有实特征值。
import math
import sys


data = sys.stdin.read().split()
it = iter(data)
a = float(next(it))
b = float(next(it))
c = float(next(it))
d = float(next(it))

t = a + d
det = a * d - b * c
disc = t * t - 4 * det
if disc < -1e-9:
    print('无实特征值')
else:
    s = math.sqrt(max(disc, 0.0))
    l1 = (t + s) / 2
    l2 = (t - s) / 2
    if l1 < l2:
        l1, l2 = l2, l1
    print('特征值 = %.3f, %.3f' % (l1, l2))
