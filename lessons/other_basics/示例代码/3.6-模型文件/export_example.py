# 仅示意：运行需要安装 PyTorch。
import torch

model = torch.nn.Linear(4, 2)
example = torch.randn(1, 4)

torch.onnx.export(
    model,
    example,
    "model.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=17,
)
