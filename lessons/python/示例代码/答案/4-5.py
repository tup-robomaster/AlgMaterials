# 练习 4-5：按行减去首列，做按列加权的平均，并对比 keepdims。
import numpy as np


temperatures = np.array([[20, 22, 24, 26],
                         [18, 20, 22, 24],
                         [25, 25, 24, 23]], dtype=np.float32)
print("shape:", temperatures.shape)
print("base:", temperatures[:, :1].shape)
centered = temperatures - temperatures[:, :1]
print("centered:", np.round(centered, 1).tolist())

weights = np.array([1.0, 2.0, 2.0, 1.0])
weighted = np.sum(centered * weights, axis=1) / weights.sum()
print("weighted:", np.round(weighted, 2).tolist())
flat = temperatures.reshape(-1)
print("flat:", flat.shape, float(np.mean(flat)))
print("keepdims:", np.mean(temperatures, axis=1, keepdims=True).shape)
