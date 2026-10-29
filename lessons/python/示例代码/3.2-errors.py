# 用自定义异常报告坏行，逐条处理，最后汇总成功与失败数量。
class ScoreError(ValueError):
    """分数行格式不对。"""


def parse_score(line):
    parts = line.split(",")
    if len(parts) != 2:
        raise ScoreError(f"expected 2 fields, got {len(parts)}")
    name, text = parts
    name = name.strip()
    if not name:
        raise ScoreError("empty name")
    try:
        score = int(text)
    except ValueError as error:
        raise ScoreError(f"score is not an integer: {text.strip()}") from error
    if not 0 <= score <= 100:
        raise ScoreError(f"score out of range: {score}")
    return name, score


lines = ["alpha,92", "beta,oops", "", "gamma,105", "delta,66"]
ok = []
failed = []
for number, line in enumerate(lines, start=1):
    try:
        ok.append(parse_score(line))
    except ScoreError as error:
        failed.append((number, str(error)))
    finally:
        print("checked:", number)

print("ok:", ok)
print("failed:", len(failed))
for number, message in failed:
    print(" ", number, message)
print("counts:", len(ok), len(failed))
