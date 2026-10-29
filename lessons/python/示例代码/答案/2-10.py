# 练习 2-10：写一个记录每次结果的装饰器，并用 wraps 保留原函数名。
from functools import wraps


def recorded(func):
    history = []

    @wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        history.append(result)
        return result

    wrapper.history = history
    return wrapper


@recorded
def add(a, b):
    return a + b


print(add(1, 2))
print(add(5, 6))
print("name:", add.__name__)
print("history:", add.history)
