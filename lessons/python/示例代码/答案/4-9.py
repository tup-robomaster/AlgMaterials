# 练习 4-9：解二元方程组，求行列式与逆矩阵，并核对单位阵。
import numpy as np


A = np.array([[4.0, 2.0], [1.0, 3.0]])
b = np.array([10.0, 8.0])
x = np.linalg.solve(A, b)
print("x:", np.round(x, 2).tolist())
print("det:", round(float(np.linalg.det(A)), 2))
print("inv:", np.round(np.linalg.inv(A), 2).tolist())
print("check:", bool(np.allclose(A @ x, b)))
print("identity:", bool(np.allclose(A @ np.linalg.inv(A), np.eye(2))))
