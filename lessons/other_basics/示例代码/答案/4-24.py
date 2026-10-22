# 4-24 求条件数
# 条件数 = σ1 / σ2；较小的奇异值为 0 时，矩阵不可逆，条件数记为无穷。
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

if s2 < 1e-12:
    print('条件数 = 无穷')
else:
    print('条件数 = %.3f' % (s1 / s2))
