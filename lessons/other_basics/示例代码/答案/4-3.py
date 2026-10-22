# 4-3 矩阵乘向量
# 结果的第 i 个分量是 A 的第 i 行与 x 的内积。
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def show(v):
    return '[' + ', '.join(f(x) for x in v) + ']'


data = sys.stdin.read().split()
it = iter(data)
m = int(next(it))
n = int(next(it))
A = [[float(next(it)) for _ in range(n)] for _ in range(m)]
x = [float(next(it)) for _ in range(n)]

y = [sum(A[i][j] * x[j] for j in range(n)) for i in range(m)]
print(show(y))
