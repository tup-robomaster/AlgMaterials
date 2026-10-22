# 4-1 向量加法与数乘
# 读入维数、两个向量和一个标量，输出 a+b 与 k*a。
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def show(v):
    return '[' + ', '.join(f(x) for x in v) + ']'


data = sys.stdin.read().split()
it = iter(data)
n = int(next(it))
a = [float(next(it)) for _ in range(n)]
b = [float(next(it)) for _ in range(n)]
k = float(next(it))

print(show([x + y for x, y in zip(a, b)]))
print(show([k * x for x in a]))
