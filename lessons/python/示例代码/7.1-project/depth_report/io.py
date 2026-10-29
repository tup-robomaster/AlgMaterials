# 读取 CSV 测量记录，跳过表头、空行与字段不足的行。
def read_measurements(path):
    rows = []
    with open(path, encoding="utf-8") as handle:
        header = handle.readline().strip().split(",")
        if header != ["name", "depth", "area"]:
            raise ValueError("unexpected header")
        for line in handle:
            line = line.strip()
            if not line:
                continue
            parts = line.split(",")
            if len(parts) != 3:
                continue
            name, depth_text, area_text = parts
            try:
                depth = float(depth_text)
                area = float(area_text)
            except ValueError:
                continue
            rows.append((name, depth, area))
    return rows
