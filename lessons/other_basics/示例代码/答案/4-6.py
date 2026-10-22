# 4-6 三点求三角形面积
# 面积 = |(P2-P1) × (P3-P1)| / 2，二维叉积就是那两项之差。
import sys


data = sys.stdin.read().split()
it = iter(data)
x1 = float(next(it))
y1 = float(next(it))
x2 = float(next(it))
y2 = float(next(it))
x3 = float(next(it))
y3 = float(next(it))

area = abs((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)) / 2
print('面积 = %.3f' % area)
