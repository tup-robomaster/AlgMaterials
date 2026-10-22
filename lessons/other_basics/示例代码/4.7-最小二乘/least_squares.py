def fit(points, through_origin=False):
    n = len(points)
    sx = sum(x for x, _ in points)
    sy = sum(y for _, y in points)
    sxx = sum(x * x for x, _ in points)
    sxy = sum(x * y for x, y in points)
    if through_origin:
        a = sxy / sxx
        b = 0.0
    else:
        det = n * sxx - sx * sx
        a = (n * sxy - sx * sy) / det
        b = (sy - a * sx) / n
    sse = sum((y - (a * x + b)) ** 2 for x, y in points)
    return a, b, sse


points = [(1.0, 2.0), (2.0, 3.0), (3.0, 5.0), (4.0, 7.0)]
a, b, sse = fit(points)

print('a = %.3f, b = %.3f' % (a, b))
print('残差平方和 = %.3f' % sse)
print('x = 5 时的预测值 = %.3f' % (a * 5 + b))
