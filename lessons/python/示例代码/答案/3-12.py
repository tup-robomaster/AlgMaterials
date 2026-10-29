# 练习 3-12：用 logger.exception 把异常与回溯写进文件，读回首尾两行。
import logging
from pathlib import Path

path = Path("out_error.log")
logger = logging.getLogger("errors")
logger.setLevel(logging.ERROR)
handler = logging.FileHandler(path, mode="w", encoding="utf-8")
handler.setFormatter(logging.Formatter("%(levelname)s %(message)s"))
logger.handlers[:] = [handler]
logger.propagate = False

try:
    value = int("12x")
except ValueError:
    logger.exception("parse failed")

logging.shutdown()
lines = path.read_text(encoding="utf-8").splitlines()
print("first:", lines[0])
print("traceback:", lines[1].startswith("Traceback"))
print("last:", lines[-1])
