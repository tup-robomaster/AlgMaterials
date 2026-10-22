import math


def dot(u, v):
    return sum(x * y for x, y in zip(u, v))


def length(u):
    return math.sqrt(dot(u, u))


def mat_vec(M, x):
    return [dot(row, x) for row in M]


def mat_mul(X, Y):
    cols = list(zip(*Y))
    return [[dot(row, col) for col in cols] for row in X]


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def show_vec(v):
    return '[' + ', '.join(f(x) for x in v) + ']'


def show_mat(M):
    return '[' + ', '.join(show_vec(row) for row in M) + ']'


a = [1.0, 2.0]
b = [3.0, 4.0]
A = [[1.0, 2.0], [3.0, 4.0]]
B = [[2.0, 1.0], [0.0, 3.0]]

print('点积 = ' + f(dot(a, b)))
print('a 的长度 = ' + f(length(a)))
print('A b = ' + show_vec(mat_vec(A, b)))
print('A B = ' + show_mat(mat_mul(A, B)))
print('B A = ' + show_mat(mat_mul(B, A)))
