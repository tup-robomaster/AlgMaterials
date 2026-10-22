# 4-18 幂迭代求主特征值
# 每步乘一次 A 并归一化，用瑞利商估计特征值；100 步足够收敛。
import math
import sys


data = sys.stdin.read().split()
it = iter(data)
A = [[float(next(it)) for _ in range(3)] for _ in range(3)]
v = [float(next(it)) for _ in range(3)]

lam = 0.0
for _ in range(100):
    w = [sum(A[i][j] * v[j] for j in range(3)) for i in range(3)]
    lam = sum(v[i] * w[i] for i in range(3)) / sum(x * x for x in v)
    n = math.sqrt(sum(x * x for x in w))
    v = [x / n for x in w]

print('主特征值 = %.3f' % lam)
