# 4-4 2×2 行列式与可逆判断
# 行列式不为 0 就可逆。用 Fraction 保证小数输入也算得准。
import sys
from fractions import Fraction


def f(x):
    s = '%.3f' % float(x)
    return '0.000' if s == '-0.000' else s


def num(q):
    if q.denominator == 1:
        return str(q.numerator)
    return f(q)


data = sys.stdin.read().split()
it = iter(data)
a = Fraction(next(it))
b = Fraction(next(it))
c = Fraction(next(it))
d = Fraction(next(it))
det = a * d - b * c

print('行列式 = ' + num(det))
print('可逆' if det != 0 else '不可逆')
