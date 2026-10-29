# 用 unittest 测试上下限裁剪函数的内部、两侧与非法区间。
import io
import unittest


def clamp(value, low, high):
    if low > high:
        raise ValueError("bad range")
    return max(low, min(value, high))


class ClampTests(unittest.TestCase):
    def test_inside(self):
        self.assertEqual(clamp(3, 0, 10), 3)

    def test_low(self):
        self.assertEqual(clamp(-2, 0, 10), 0)

    def test_high(self):
        self.assertEqual(clamp(20, 0, 10), 10)

    def test_bad_range(self):
        with self.assertRaises(ValueError):
            clamp(5, 10, 0)


suite = unittest.defaultTestLoader.loadTestsFromTestCase(ClampTests)
stream = io.StringIO()
result = unittest.TextTestRunner(stream=stream, verbosity=1).run(suite)
print("tests run:", result.testsRun)
print("failures:", len(result.failures))
print("ok:", result.wasSuccessful())
print("values:", [clamp(-2, 0, 10), clamp(3, 0, 10), clamp(20, 0, 10)])
