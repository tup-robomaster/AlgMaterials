import math


def eig2(M):
    a, b = M[0]
    c, d = M[1]
    t = a + d
    det = a * d - b * c
    s = math.sqrt(max(t * t - 4 * det, 0.0))
    return [(t + s) / 2, (t - s) / 2]


def eigvec2(M, lam):
    a, b = M[0]
    c, d = M[1]
    if abs(b) > 1e-12:
        v = [b, lam - a]
    elif abs(c) > 1e-12:
        v = [lam - d, c]
    elif abs(a - lam) < 1e-12:
        v = [1.0, 0.0]
    else:
        v = [0.0, 1.0]
    n = math.sqrt(v[0] * v[0] + v[1] * v[1])
    v = [v[0] / n, v[1] / n]
    for x in v:
        if abs(x) > 1e-12:
            if x < 0:
                v = [-v[0], -v[1]]
            break
    return v


def power(A, v0, steps=100):
    v = list(v0)
    lam = 0.0
    for _ in range(steps):
        w = [sum(A[i][j] * v[j] for j in range(len(v)))
             for i in range(len(v))]
        lam = sum(v[i] * w[i] for i in range(len(v))) / \
            sum(x * x for x in v)
        n = math.sqrt(sum(x * x for x in w))
        v = [x / n for x in w]
    return lam


A = [[2.0, 1.0], [1.0, 2.0]]
lams = eig2(A)
v1 = eigvec2(A, lams[0])
v2 = eigvec2(A, lams[1])
Q = [[v1[0], v2[0]], [v1[1], v2[1]]]
QlQ = [[sum(Q[i][k] * lams[k] * Q[j][k] for k in range(2))
        for j in range(2)] for i in range(2)]
B = [[4.0, 1.0, 0.0], [1.0, 3.0, 1.0], [0.0, 1.0, 2.0]]

print('特征值 = %.3f, %.3f' % (lams[0], lams[1]))
print('较大特征值的单位特征向量 = [%.3f, %.3f]' % (v1[0], v1[1]))
print('Q Λ Qᵀ = [[%.3f, %.3f], [%.3f, %.3f]]' %
      (QlQ[0][0], QlQ[0][1], QlQ[1][0], QlQ[1][1]))
print('幂迭代的主特征值 = %.3f' % power(B, [1.0, 1.0, 1.0]))
