# 练习 2-2：用闭包返回两个互不共享状态的计数器。
def make_counter(start=0, step=1):
    count = start

    def counter():
        nonlocal count
        count += step
        return count

    return counter


first = make_counter()
second = make_counter(10, 5)
print(first(), first())
print(second())
print(first())
