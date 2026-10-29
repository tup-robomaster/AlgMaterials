# 解三元线性方程组，再用最小二乘拟合一条直线。
import numpy as np


A = np.array([[2.0, 1.0, -1.0],
              [-3.0, -1.0, 2.0],
              [-2.0, 1.0, 2.0]])
b = np.array([8.0, -11.0, -3.0])

x = np.linalg.solve(A, b)
print("x:", np.round(x, 3).tolist())
print("residual:", np.round(np.linalg.norm(A @ x - b), 6))
print("det:", round(float(np.linalg.det(A)), 3))
print("inv:", np.round(np.linalg.inv(A), 2).tolist())
print("eye:", bool(np.allclose(A @ np.linalg.inv(A), np.eye(3))))

t = np.array([0.0, 1.0, 2.0, 3.0, 4.0])
y = np.array([1.1, 2.9, 5.2, 6.8, 9.1])
design = np.column_stack([t, np.ones_like(t)])
coef, _, rank, _ = np.linalg.lstsq(design, y, rcond=None)
print("fit:", np.round(coef, 3).tolist(), "rank:", int(rank))
print("pred:", np.round(design @ coef, 2).tolist())
print("coef norm:", round(float(np.linalg.norm(coef)), 3))

u, s, vt = np.linalg.svd(design, full_matrices=False)
print("svd:", np.round(s, 3).tolist(), u.shape, vt.shape)
