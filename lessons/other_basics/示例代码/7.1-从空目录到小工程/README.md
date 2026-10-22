# camera-demo

读取 `camera.json` 中的相机 A 参数，计算视场角、单帧大小与数据量，
结果写入 `report.json`。

## 运行

```bash
chmod +x run.sh
./run.sh
```

`report.json` 是生成文件，不提交到仓库。

## 文件

- `camera.json`：相机参数
- `compute.py`：计算脚本
- `run.sh`：启动脚本
