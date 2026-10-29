# 查看常见类型的名字，以及整数、浮点数与字符串之间的转换。
#
# 运行：在课程根目录执行 python3 示例代码/1.1-types.py
values = [42, 3.5, True, None, "42", [1, 2]]

for value in values:
    print(f"{str(value):<8} {type(value).__name__:<8} {isinstance(value, int)}")

print("bool is int:", isinstance(True, int))
print("big:", 2 ** 100)
print("int('42') + 1 =", int("42") + 1)
print("float('3.5') * 2 =", float("3.5") * 2)
print("round(2.675, 2) =", round(2.675, 2))
