"""生成 8.3 的节点与话题拓扑图。运行后写出 rqt-graph.png。"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

WIDTH, HEIGHT = 900, 520
BG = (247, 249, 252)
INK = (32, 39, 51)
MUTED = (91, 101, 117)
BLUE = (45, 108, 223)
BLUE_BG = (232, 240, 255)
GREEN = (31, 138, 91)
GREEN_BG = (231, 247, 238)
ARROW = (108, 119, 136)

font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
bold_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
font = ImageFont.truetype(font_path, 26)
small = ImageFont.truetype(font_path, 20)
tiny = ImageFont.truetype(font_path, 17)
bold = ImageFont.truetype(bold_path, 28)

image = Image.new("RGB", (WIDTH, HEIGHT), BG)
draw = ImageDraw.Draw(image)


def node_box(x, y, w, h, title, subtitle, fill, outline):
    draw.rounded_rectangle((x, y, x + w, y + h), radius=18, fill=fill, outline=outline, width=3)
    draw.text((x + w / 2, y + 42), title, font=bold, fill=INK, anchor="mm")
    draw.text((x + w / 2, y + 84), subtitle, font=small, fill=MUTED, anchor="mm")


def arrow(x1, y1, x2, y2):
    draw.line((x1, y1, x2, y2), fill=ARROW, width=4)
    draw.polygon(
        [(x2, y2), (x2 - 16, y2 - 9), (x2 - 16, y2 + 9)],
        fill=ARROW,
    )


node_box(90, 170, 230, 130, "/talker", "publisher", BLUE_BG, BLUE)
node_box(580, 170, 230, 130, "/listener", "subscriber", GREEN_BG, GREEN)

arrow(330, 235, 570, 235)
draw.text((450, 175), "/chatter", font=font, fill=INK, anchor="mm")
draw.text((450, 285), "std_msgs/msg/String", font=tiny, fill=MUTED, anchor="mm")

draw.text((WIDTH / 2, 70), "rqt_graph: nodes and topics", font=bold, fill=INK, anchor="mm")
draw.text(
    (WIDTH / 2, 440),
    "The arrow points from the publisher to the subscriber.",
    font=small,
    fill=MUTED,
    anchor="mm",
)

out = Path(__file__).with_name("rqt-graph.png")
image.save(out)
print(out)
