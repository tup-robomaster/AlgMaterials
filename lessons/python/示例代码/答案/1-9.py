# 练习 1-9：用条件与推导式把测量值分成警告区和安全区。
values = [3, 18, 25, 40, 7, 33]
warn = [value for value in values if 10 <= value <= 30]
safe = [value for value in values if value < 10 or value > 30]
print("warn:", warn)
print("safe:", safe)
print("above 20:", sum(1 for value in values if value > 20))
