# 把统计摘要排成一行文本。
def format_summary(name, summary):
    count, average, top = summary
    return f"{name}: n={count} mean={average:.2f} max={top}"
