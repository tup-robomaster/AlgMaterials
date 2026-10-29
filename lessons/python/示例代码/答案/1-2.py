# 练习 1-2：打印值的类型名与是否为数字，并观察浮点加法误差。
values = [7, 2.5, False, "7", None]

for value in values:
    is_number = isinstance(value, (int, float)) and not isinstance(value, bool)
    print(f"{str(value):<8}{type(value).__name__:<9}{is_number}")

print("0.1 + 0.2 == 0.3:", 0.1 + 0.2 == 0.3)
print("rounded:", round(0.1 + 0.2, 2))
