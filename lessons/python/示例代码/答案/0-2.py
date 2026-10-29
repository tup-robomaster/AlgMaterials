# 练习 0-2：检查课程根目录下的两个目录是否存在。
from pathlib import Path

for name in sorted(["素材", "示例代码"]):
    print(f"目录 {name}: {Path(name).is_dir()}")
