# 练习 3-2：创建父目录，把文本转成大写后写到新文件，再读回核对。
from pathlib import Path

out = Path("out/notes_copy.txt")
out.parent.mkdir(parents=True, exist_ok=True)
source = Path("素材/synth_notes.txt")
text = source.read_text(encoding="utf-8")
out.write_text(text.upper(), encoding="utf-8")
print("exists:", out.exists())
print("lines:", len(out.read_text(encoding="utf-8").splitlines()))
print("first:", out.read_text(encoding="utf-8").splitlines()[0])
print("size:", out.stat().st_size)
