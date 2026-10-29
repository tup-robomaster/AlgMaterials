# 练习 1-3：按逗号与等号拆开配置文本，去掉两端的空格后对齐打印。
text = "  exposure=12, gain=3.5 ,gamma=1.0 "
for part in text.split(","):
    key, value = part.split("=")
    print(f"{key.strip():<10}{value.strip()}")
