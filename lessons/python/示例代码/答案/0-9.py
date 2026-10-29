# 练习 0-9：验证赋值是别名，切片才得到独立列表。
a = [1, 2, 3]
b = a
b.append(4)
print("a:", a)

c = a[:]
c.append(5)
print("a:", a)
print("c:", c)
