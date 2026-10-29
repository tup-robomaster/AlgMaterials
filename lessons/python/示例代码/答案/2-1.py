# 练习 2-1：仅位置参数与仅关键字参数各用一次，并观察传错时的异常。
def format_measure(value, /, unit, *, digits=1):
    return f"{value:.{digits}f} {unit}"


print(format_measure(3.14159, "V"))
print(format_measure(2.5, "A", digits=0))
try:
    format_measure(value=1.0, unit="V")
except TypeError:
    print("rejected: TypeError")
