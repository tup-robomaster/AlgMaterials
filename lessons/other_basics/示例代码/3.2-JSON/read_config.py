import json

with open("config.json", encoding="utf-8") as f:
    config = json.load(f)

camera = config["camera"]
print("相机:", camera["name"])
print("分辨率:", camera["resolution"][0], "x", camera["resolution"][1])
print("帧率:", camera["fps"])
print("曝光:", camera["exposure_us"], "us")
print("触发:", camera["trigger"])
