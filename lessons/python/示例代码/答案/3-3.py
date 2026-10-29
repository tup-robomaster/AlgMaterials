# 练习 3-3：把转换失败与除零都收成 None，并打印除零异常的消息。
def parse_int(text):
    try:
        return int(text)
    except ValueError:
        return None


def safe_div(a, b):
    try:
        return a / b
    except ZeroDivisionError:
        return None


values = ["12", "x", "0", "7"]
print("parsed:", [parse_int(text) for text in values])
print("div:", safe_div(10, 2), safe_div(1, 0))
try:
    5 / 0
except ZeroDivisionError as error:
    print("message:", str(error))
