import math


def trans(deg, tx, ty):
    t = math.radians(deg)
    c, s = math.cos(t), math.sin(t)
    return [[c, -s, tx], [s, c, ty], [0.0, 0.0, 1.0]]


def mul(A, B):
    return [[sum(A[i][k] * B[k][j] for k in range(3)) for j in range(3)]
            for i in range(3)]


def apply(T, p):
    return [T[0][0] * p[0] + T[0][1] * p[1] + T[0][2],
            T[1][0] * p[0] + T[1][1] * p[1] + T[1][2]]


def inverse(T):
    R = [[T[0][0], T[0][1]], [T[1][0], T[1][1]]]
    t = [T[0][2], T[1][2]]
    Rt = [[R[0][0], R[1][0]], [R[0][1], R[1][1]]]
    ti = [-(Rt[0][0] * t[0] + Rt[0][1] * t[1]),
          -(Rt[1][0] * t[0] + Rt[1][1] * t[1])]
    return [[Rt[0][0], Rt[0][1], ti[0]],
            [Rt[1][0], Rt[1][1], ti[1]],
            [0.0, 0.0, 1.0]]


def f(x):
    s = '%.3f' % x
    return '0.000' if s == '-0.000' else s


def show_vec(v):
    return '[' + ', '.join(f(x) for x in v) + ']'


p = [1.0, 0.0]
R90 = trans(90, 0, 0)
Tt = trans(0, 2, 1)
T1 = mul(Tt, R90)
T2 = trans(180, 1, 0)

print('旋转 90° = ' + show_vec(apply(R90, p)))
print('旋转后平移 = ' + show_vec(apply(T1, p)))
print('复合矩阵作用于同一点 = ' + show_vec(apply(mul(Tt, R90), p)))
print('逆变换送回 = ' + show_vec(apply(inverse(T1), apply(T1, p))))
print('先 T1 再 T2 = ' + show_vec(apply(mul(T2, T1), p)))
