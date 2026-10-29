# 练习 2-5：同一份文件既当程序运行，又用 runpy 以另一个模块名加载。
import runpy


def mean(values):
    return sum(values) / len(values)


if __name__ == "__main__":
    print("main:", __name__)
    namespace = runpy.run_path(__file__, run_name="tool")
    print("tool mean:", namespace["mean"]([1, 2, 3]))
    print("names:", sorted(n for n in namespace if not n.startswith("__")))
