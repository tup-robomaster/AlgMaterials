from fractions import Fraction


def transpose(M):
    return [[M[j][i] for j in range(len(M))] for i in range(len(M[0]))]


def inverse(M):
    n = len(M)
    A = []
    for i in range(n):
        A.append([Fraction(M[i][j]) for j in range(n)] +
                 [Fraction(1 if i == j else 0) for j in range(n)])
    for c in range(n):
        p = None
        for i in range(c, n):
            if A[i][c] != 0:
                p = i
                break
        if p is None:
            return None
        A[c], A[p] = A[p], A[c]
        v = A[c][c]
        A[c] = [x / v for x in A[c]]
        for i in range(n):
            if i != c and A[i][c] != 0:
                k = A[i][c]
                A[i] = [x - k * y for x, y in zip(A[i], A[c])]
    return [row[n:] for row in A]


def f(x):
    s = '%.3f' % float(x)
    return '0.000' if s == '-0.000' else s


def show(M):
    return '[' + ', '.join(
        '[' + ', '.join(f(x) for x in row) + ']' for row in M) + ']'


A2 = [[2, 1], [1, 3]]
A3 = [[2, 1, 0], [1, 2, 1], [0, 1, 2]]
R = [[0, -1], [1, 0]]
RtR = [[sum(R[k][i] * R[k][j] for k in range(2)) for j in range(2)]
       for i in range(2)]

print('A2 的逆：')
print(show(inverse(A2)))
print('A3 的逆：')
print(show(inverse(A3)))
print('RᵀR = ' + show(RtR))
