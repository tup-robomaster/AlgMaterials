# 4-19 直线拟合
# 拟合 y = a x + b，正规方程是 [n Σx; Σx Σx²] [b; a] = [Σy; Σxy]。
import sys


data = sys.stdin.read().split()
it = iter(data)
n = int(next(it))
pts = [(float(next(it)), float(next(it))) for _ in range(n)]

sx = sum(x for x, _ in pts)
sy = sum(y for _, y in pts)
sxx = sum(x * x for x, _ in pts)
sxy = sum(x * y for x, y in pts)

a = (n * sxy - sx * sy) / (n * sxx - sx * sx)
b = (sy - a * sx) / n
sse = sum((y - (a * x + b)) ** 2 for x, y in pts)

print('a = %.3f' % a)
print('b = %.3f' % b)
print('残差平方和 = %.3f' % sse)
