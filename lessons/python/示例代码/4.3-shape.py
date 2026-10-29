# 改变数组形状，并用广播把一维偏置加到每一行。
import cv2
import numpy as np


a = np.arange(24, dtype=np.float32).reshape(2, 3, 4)
print("a:", a.shape, a.size)
print("transpose:", a.transpose(1, 0, 2).shape)
print("squeeze:", np.squeeze(a[:, :1, :]).shape)
print("expand:", np.expand_dims(a, axis=0).shape)

rows = a.reshape(6, 4)
print("reshape:", rows.shape, rows[0].tolist())
joined = np.concatenate([rows, rows], axis=0)
stacked = np.stack([rows, rows], axis=0)
print("join/stack:", joined.shape, stacked.shape)

bias = np.array([10.0, 0.0, -10.0, 5.0])
print("bias:", np.round(rows + bias, 1)[0].tolist())
grouped = rows.reshape(2, 3, 4)
print("axis mean:", np.round(grouped.mean(axis=1), 2).tolist())

image = cv2.imread("素材/synth_shapes.png")
print("image:", image.shape, image.dtype)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY).astype(np.float32)
normalized = (gray - gray.mean()) / gray.std()
print("norm:", round(float(normalized.mean()), 4), round(float(normalized.std()), 4))

scale = np.array([1.0, 1.1, 0.9], dtype=np.float32)
adjusted = np.clip(image.astype(np.float32) * scale, 0, 255).astype(np.uint8)
means = adjusted.reshape(-1, 3).mean(axis=0)
print("channel means:", np.round(means, 2).tolist())
