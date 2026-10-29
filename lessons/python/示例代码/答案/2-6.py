# 练习 2-6：在临时目录写两个模块，导入并观察模块顶层的 __name__。
import sys
import tempfile
from pathlib import Path

with tempfile.TemporaryDirectory() as tmp:
    folder = Path(tmp)
    (folder / "helpers.py").write_text(
        "def double(value):\n    return value * 2\n", encoding="utf-8")
    (folder / "main.py").write_text(
        "from helpers import double\n\n"
        "print('imported:', double(21))\n"
        "print('name:', __name__)\n", encoding="utf-8")

    sys.path.insert(0, str(folder))
    try:
        import helpers
        print("helpers:", helpers.double(5))
        import main
    finally:
        sys.path.remove(str(folder))
        sys.modules.pop("helpers", None)
        sys.modules.pop("main", None)
