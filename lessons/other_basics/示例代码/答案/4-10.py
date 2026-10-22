# 4-10 求转置
# 转置把第 i 行第 j 列换到第 j 行第 i 列。
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def show_mat(M):
    return '[' + ', '.join(
        '[' + ', '.join(f(x) for x in row) + ']' for row in M) + ']'


data = sys.stdin.read().split()
it = iter(data)
m = int(next(it))
n = int(next(it))
M = [[float(next(it)) for _ in range(n)] for _ in range(m)]

T = [[M[i][j] for i in range(m)] for j in range(n)]
print(show_mat(T))
