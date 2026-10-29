# 交卷前的自检形状：读一张固定图片，用 unittest 核对尺寸、通道与均值。
# 五道检测题都可以照这个结构写：给定输入、断言输出、失败时退出非零。
import io
import sys
import unittest
from pathlib import Path

import cv2

SOURCE = Path("素材/synth_shapes.png")


def image_stats(path):
    image = cv2.imread(str(path))
    if image is None:
        raise SystemExit("cannot read image: %s" % path)
    return {
        "height": image.shape[0],
        "width": image.shape[1],
        "channels": image.shape[2],
        "mean": round(float(image.mean()), 2),
    }


class StatsTest(unittest.TestCase):
    def setUp(self):
        self.data = image_stats(SOURCE)

    def test_size(self):
        size = (self.data["width"], self.data["height"])
        self.assertEqual(size, (640, 480))

    def test_channels(self):
        self.assertEqual(self.data["channels"], 3)


def main():
    data = image_stats(SOURCE)
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(StatsTest)
    stream = io.StringIO()
    result = unittest.TextTestRunner(stream=stream, verbosity=0).run(suite)
    print("size: %dx%d" % (data["width"], data["height"]))
    print("channels:", data["channels"])
    print("mean: %.2f" % data["mean"])
    print("tests:", "OK" if result.wasSuccessful() else "FAILED")
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    sys.exit(main())
