# 4-11 2×2 求逆
# 行列式为 0 时没有逆矩阵；否则用 [[d,-b],[-c,a]] 除以行列式。
import sys
from fractions import Fraction


def f(x):
    s = '%.3f' % float(x)
    return '0.000' if s == '-0.000' else s


def show_mat(M):
    return '[' + ', '.join(
        '[' + ', '.join(f(x) for x in row) + ']' for row in M) + ']'


data = sys.stdin.read().split()
it = iter(data)
a = Fraction(next(it))
b = Fraction(next(it))
c = Fraction(next(it))
d = Fraction(next(it))
det = a * d - b * c

if det == 0:
    print('不可逆')
else:
    print(show_mat([[d / det, -b / det], [-c / det, a / det]]))
