# 打印练习目录的信息：当前目录、脚本目录与素材目录是否就位。
#
# 运行：在课程根目录执行 python3 示例代码/0.1-check.py
from pathlib import Path

print("当前目录:", Path.cwd().name)
print("脚本目录:", Path(__file__).resolve().parent.name)
print("素材目录存在:", (Path(__file__).resolve().parent.parent / "素材").is_dir())
