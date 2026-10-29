# 用 unittest 测试中位数的奇数、偶数与空列表情况。
import io
import unittest


def median(values):
    if not values:
        raise ValueError("empty")
    ordered = sorted(values)
    middle = len(ordered) // 2
    if len(ordered) % 2:
        return float(ordered[middle])
    return (ordered[middle - 1] + ordered[middle]) / 2.0


class MedianTests(unittest.TestCase):
    def test_odd(self):
        self.assertEqual(median([3, 1, 2]), 2.0)

    def test_even(self):
        self.assertEqual(median([4, 1, 3, 2]), 2.5)

    def test_empty(self):
        with self.assertRaises(ValueError):
            median([])


suite = unittest.defaultTestLoader.loadTestsFromTestCase(MedianTests)
stream = io.StringIO()
result = unittest.TextTestRunner(stream=stream, verbosity=2).run(suite)
print("tests run:", result.testsRun)
print("failures:", len(result.failures))
print("errors:", len(result.errors))
print("ok:", result.wasSuccessful())
print("first line:", stream.getvalue().splitlines()[0])
