# 练习 1-4：拆分、连接与切分字符串，检查成员关系。
text = "  vision,robot,armor  "
parts = text.strip().split(",")
print("-".join(parts).upper())
print(parts[0][:3])
print(parts[2][-2:])
print("contains:", "armor" in parts)
