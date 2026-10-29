# 包内入口：用绝对导入取兄弟模块，打印一组固定分数的摘要。
from toolbox.report import format_summary
from toolbox.stats import summarize

scores = [88, 92, 79, 95, 84]
summary = summarize(scores)
print(format_summary("scores", summary))
print("module:", __name__)
