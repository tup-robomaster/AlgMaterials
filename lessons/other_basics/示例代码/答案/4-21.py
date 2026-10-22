# 4-21 用拟合结果预测
# 先按正规方程求出 a、b，再把 x0 代入 y = a x + b。
import sys


data = sys.stdin.read().split()
it = iter(data)
n = int(next(it))
pts = [(float(next(it)), float(next(it))) for _ in range(n)]
x0 = float(next(it))

sx = sum(x for x, _ in pts)
sy = sum(y for _, y in pts)
sxx = sum(x * x for x, _ in pts)
sxy = sum(x * y for x, y in pts)
a = (n * sxy - sx * sy) / (n * sxx - sx * sx)
b = (sy - a * sx) / n

print('预测值 = %.3f' % (a * x0 + b))
