# 练习 2-8：给解析函数写文档字符串与标注，用 inspect 查看签名。
import inspect
from typing import get_type_hints


def parse_line(line: str, sep: str = ",") -> tuple[str, int] | None:
    """把“名字,分数”一行拆开。

    参数：
        line: 一行文本。
        sep: 字段分隔符。
    返回：
        (名字, 分数)；格式不对时返回 None。
    """
    parts = line.split(sep)
    if len(parts) != 2:
        return None
    name, text = parts
    try:
        return name.strip(), int(text)
    except ValueError:
        return None


print(parse_line("alpha, 92"))
print(parse_line("alpha"))
print("signature:", inspect.signature(parse_line))
print("hints:", get_type_hints(parse_line))
print("first doc:", parse_line.__doc__.splitlines()[0])
