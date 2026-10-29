# 对测量记录做汇总，并按深度阈值筛选。
def summarize(rows):
    if not rows:
        return {"count": 0, "mean_depth": 0.0, "max_area": 0.0}
    depths = [row[1] for row in rows]
    areas = [row[2] for row in rows]
    return {
        "count": len(rows),
        "mean_depth": sum(depths) / len(depths),
        "max_area": max(areas),
    }


def near(rows, limit):
    return [row[0] for row in rows if row[1] <= limit]
