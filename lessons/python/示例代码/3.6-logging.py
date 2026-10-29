# 用 logging 记录批处理结果：控制台只显示消息，文件里留下异常的回溯。
import logging
import sys
from pathlib import Path

LOG_PATH = Path("out_run.log")


def process(logger, file_logger, names):
    ok = skipped = 0
    for name in names:
        if not name.endswith(".py"):
            logger.warning("skip %s", name)
            skipped += 1
            continue
        if name == "bad.py":
            try:
                raise ValueError("cannot read bad.py")
            except ValueError as error:
                logger.error("failed %s: %s", name, error)
                file_logger.exception("failed %s", name)
            continue
        logger.info("ok %s", name)
        ok += 1
    return ok, skipped


def main():
    console = logging.StreamHandler(sys.stdout)
    console.setLevel(logging.INFO)
    console.setFormatter(logging.Formatter("%(levelname)s %(name)s %(message)s"))
    file_handler = logging.FileHandler(LOG_PATH, mode="w", encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)
    file_handler.setFormatter(logging.Formatter("%(levelname)s %(message)s"))

    logger = logging.getLogger("batch")
    logger.setLevel(logging.DEBUG)
    logger.handlers[:] = [console, file_handler]
    logger.propagate = False

    file_logger = logging.getLogger("batch.file")
    file_logger.setLevel(logging.DEBUG)
    file_logger.handlers[:] = [file_handler]
    file_logger.propagate = False

    ok, skipped = process(logger, file_logger, ["a.py", "notes.txt", "bad.py", "b.py"])
    logger.info("done ok=%d skipped=%d", ok, skipped)
    logging.shutdown()

    lines = LOG_PATH.read_text(encoding="utf-8").splitlines()
    print("ok:", ok, "skipped:", skipped)
    print("log first:", lines[0])
    print("log last:", lines[-1])
    print("traceback logged:", any(line.startswith("Traceback") for line in lines))


if __name__ == "__main__":
    main()
