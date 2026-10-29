# 用 assert 检查函数前提与结果，并统计执行的检查次数。
def average(values):
    assert values, "empty"
    return sum(values) / len(values)


checks = 0
assert average([2, 4, 6]) == 4.0
checks += 1
assert average([10]) == 10.0
checks += 1
try:
    average([])
except AssertionError as error:
    print("empty:", str(error))
    checks += 1
print("checks:", checks)
print("average:", average([2, 4, 6]))
