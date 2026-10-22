import math


def svd2(M):
    a, b = M[0]
    c, d = M[1]
    p = a * a + c * c
    q = a * b + c * d
    r = b * b + d * d
    t = p + r
    det = p * r - q * q
    disc = math.sqrt(max(t * t - 4 * det, 0.0))
    l1 = (t + disc) / 2
    l2 = (t - disc) / 2
    return math.sqrt(max(l1, 0.0)), math.sqrt(max(l2, 0.0))


def rank1(M):
    a, b = M[0]
    c, d = M[1]
    p = a * a + c * c
    q = a * b + c * d
    r = b * b + d * d
    t = p + r
    det = p * r - q * q
    disc = math.sqrt(max(t * t - 4 * det, 0.0))
    l1 = (t + disc) / 2
    s1 = math.sqrt(max(l1, 0.0))
    if s1 == 0:
        return [[0.0, 0.0], [0.0, 0.0]]
    if abs(q) > 1e-12:
        v = [q, l1 - p]
    elif p >= r:
        v = [1.0, 0.0]
    else:
        v = [0.0, 1.0]
    n = math.sqrt(v[0] * v[0] + v[1] * v[1])
    v = [v[0] / n, v[1] / n]
    w = [a * v[0] + b * v[1], c * v[0] + d * v[1]]
    u = [w[0] / s1, w[1] / s1]
    return [[s1 * u[i] * v[j] for j in range(2)] for i in range(2)]


A = [[4.0, 0.0], [3.0, 5.0]]
s1, s2 = svd2(A)
R = rank1(A)

print('奇异值 = %.3f, %.3f' % (s1, s2))
print('秩 1 近似 = [[%.3f, %.3f], [%.3f, %.3f]]' %
      (R[0][0], R[0][1], R[1][0], R[1][1]))
print('条件数 = %.3f' % (s1 / s2))
