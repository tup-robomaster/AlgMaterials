# 练习 0-10：验证函数会修改传入的列表，并观察负数的整除与余数。
def add_item(values, item):
    values.append(item)


values = [1, 2]
returned = add_item(values, 3)
print("values:", values)
print("returned:", returned)
print("-7 // 2 =", -7 // 2)
print("-7 % 2 =", -7 % 2)
print("empty:", bool([]), "nonempty:", bool([0]))
