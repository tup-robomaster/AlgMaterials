# 4-20 过原点拟合
# 直线过原点时 b = 0，只有一个未知量 a = Σxy / Σx²。
import sys


data = sys.stdin.read().split()
it = iter(data)
n = int(next(it))
pts = [(float(next(it)), float(next(it))) for _ in range(n)]

sxx = sum(x * x for x, _ in pts)
sxy = sum(x * y for x, y in pts)
a = sxy / sxx

print('a = %.3f' % a)
