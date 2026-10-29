# 练习 3-11：文件处理器只收 WARNING 及以上，读回文件核对留下了哪些行。
import logging
from pathlib import Path

path = Path("out_levels.log")
logger = logging.getLogger("levels")
logger.setLevel(logging.DEBUG)
handler = logging.FileHandler(path, mode="w", encoding="utf-8")
handler.setLevel(logging.WARNING)
handler.setFormatter(logging.Formatter("%(levelname)s %(message)s"))
logger.handlers[:] = [handler]
logger.propagate = False

logger.debug("debug")
logger.info("info")
logger.warning("warn")
logger.error("error")
logging.shutdown()

lines = path.read_text(encoding="utf-8").splitlines()
print("lines:", len(lines))
print(lines)
