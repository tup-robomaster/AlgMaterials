# 4-2 点积、长度与夹角
# 夹角余弦 = 点积 / (两个长度之积)。
import math
import sys


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


data = sys.stdin.read().split()
it = iter(data)
n = int(next(it))
a = [float(next(it)) for _ in range(n)]
b = [float(next(it)) for _ in range(n)]

dot = sum(x * y for x, y in zip(a, b))
la = math.sqrt(sum(x * x for x in a))
lb = math.sqrt(sum(x * x for x in b))
cos = dot / (la * lb) if la != 0 and lb != 0 else 0.0

print('点积 = ' + f(dot))
print('|a| = ' + f(la))
print('|b| = ' + f(lb))
print('cos = ' + f(cos))
