# 练习 3-6：连续写三个结果文件，再为下一个结果找不重名的路径。
from pathlib import Path

out_dir = Path("out")
out_dir.mkdir(exist_ok=True)
for index in range(1, 4):
    path = out_dir / f"result_{index}.txt"
    path.write_text(f"run {index}\n", encoding="utf-8")
    print(path.name, path.exists())

candidate = out_dir / "result_3.txt"
number = 3
while candidate.exists():
    number += 1
    candidate = out_dir / f"result_{number}.txt"
print("next:", candidate.name)
