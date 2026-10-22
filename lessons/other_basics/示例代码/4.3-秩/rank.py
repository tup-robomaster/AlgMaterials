from fractions import Fraction


def rref(M):
    A = [[Fraction(x) for x in row] for row in M]
    rows, cols = len(A), len(A[0])
    pivots = []
    r = 0
    for c in range(cols):
        p = None
        for i in range(r, rows):
            if A[i][c] != 0:
                p = i
                break
        if p is None:
            continue
        A[r], A[p] = A[p], A[r]
        v = A[r][c]
        A[r] = [x / v for x in A[r]]
        for i in range(rows):
            if i != r and A[i][c] != 0:
                k = A[i][c]
                A[i] = [x - k * y for x, y in zip(A[i], A[r])]
        pivots.append(c)
        r += 1
        if r == rows:
            break
    return A, pivots


def show(M):
    return '\n'.join('[' + ', '.join(str(x) for x in row) + ']'
                     for row in M)


def kind(M):
    R, pivots = rref(M)
    n = len(M[0]) - 1
    for row in R:
        if all(x == 0 for x in row[:n]) and row[n] != 0:
            return '无解'
    if len(pivots) < n:
        return '无穷多解'
    return '唯一解'


M = [[1, 2, 0], [2, 4, 1], [0, 0, 1]]
R, pivots = rref(M)
print('行简化：')
print(show(R))
print('秩 = %d' % len(pivots))
print('增广矩阵的解：' + kind([[1, 1, 3], [2, -1, 0]]))
