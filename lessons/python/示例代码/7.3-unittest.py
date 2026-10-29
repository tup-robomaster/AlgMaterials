# 用 unittest 为平均值函数写正常、单值与空列表测试，并打印确定性的摘要。
import io
import unittest


def mean(values):
    if not values:
        raise ValueError("empty")
    return sum(values) / len(values)


class MeanTests(unittest.TestCase):
    def test_typical(self):
        self.assertAlmostEqual(mean([2, 4, 6]), 4.0)

    def test_single(self):
        self.assertEqual(mean([7]), 7.0)

    def test_empty(self):
        with self.assertRaises(ValueError):
            mean([])


def run_suite():
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(MeanTests)
    stream = io.StringIO()
    result = unittest.TextTestRunner(stream=stream, verbosity=2).run(suite)
    print("tests run:", result.testsRun)
    print("failures:", len(result.failures))
    print("errors:", len(result.errors))
    print("skipped:", len(result.skipped))
    print("ok:", result.wasSuccessful())
    print("first line:", stream.getvalue().splitlines()[0])


if __name__ == "__main__":
    run_suite()
