# 演示 C++ 读者最容易沿用的五个习惯在 Python 里会怎样。
#
# 运行：在课程根目录执行 python3 示例代码/0.5-demo.py
def add_one(values):
    values.append(1)


a = [1, 2, 3]
b = a
b.append(4)
print("alias:", a)

c = a[:]
c.append(5)
print("copy:", a)
print("copy:", c)

add_one(a)
print("mutated:", a)

print("7 / 2 =", 7 / 2)
print("7 // 2 =", 7 // 2)
print("bool:", bool([]), bool([0]), bool(""))

x = None
print("none:", x is None)
