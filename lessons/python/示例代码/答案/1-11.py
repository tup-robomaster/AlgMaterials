# 练习 1-11：滑动平均与限幅。窗口长度不合法时抛出 ValueError。
def moving_average(values, window):
    if window <= 0:
        raise ValueError("window must be positive")
    result = []
    for index in range(len(values)):
        start = max(0, index - window + 1)
        chunk = values[start:index + 1]
        result.append(sum(chunk) / len(chunk))
    return result


def limit(value, low, high):
    return max(low, min(value, high))


values = [10, 12, 16, 14, 20, 18]
smoothed = moving_average(values, 3)
print([round(value, 2) for value in smoothed])
print(limit(1.5, 0.0, 1.0), limit(-2, 0, 1))
