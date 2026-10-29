# 练习 4-10：用最小二乘拟合直线，查看预测值、误差与奇异值。
import numpy as np


t = np.array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0])
y = np.array([2.0, 4.1, 6.2, 7.9, 10.1, 12.2])
design = np.column_stack([t, np.ones_like(t)])
coef, _, _, _ = np.linalg.lstsq(design, y, rcond=None)
print("slope/intercept:", np.round(coef, 2).tolist())
print("pred:", np.round(design @ coef, 2).tolist())
print("error:", round(float(np.linalg.norm(design @ coef - y)), 3))

_, s, _ = np.linalg.svd(design, full_matrices=False)
print("svd:", np.round(s, 2).tolist())
print("rank:", int(np.linalg.matrix_rank(design)))
