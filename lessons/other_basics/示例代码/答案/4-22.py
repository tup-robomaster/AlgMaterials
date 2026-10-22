# 4-22 求两个奇异值
# 奇异值是 AᵀA 的两个特征值开平方，降序输出。
import math
import sys


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
s1 = math.sqrt(max((t + disc) / 2, 0.0))
s2 = math.sqrt(max((t - disc) / 2, 0.0))

print('奇异值 = %.3f, %.3f' % (s1, s2))
