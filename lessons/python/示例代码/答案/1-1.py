# 练习 1-1：把字符串形式的摄氏温度转换成浮点数，再换算成华氏温度。
celsius = ["21.5", "0", "-3.5"]
for text in celsius:
    value = float(text)
    print(f"{text:>6} C = {value * 9 / 5 + 32:6.1f} F")
