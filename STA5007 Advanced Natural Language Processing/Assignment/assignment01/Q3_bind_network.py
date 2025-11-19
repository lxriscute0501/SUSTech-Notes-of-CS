import torch
import torch.nn as nn
import torch.nn.functional as F

class BindNetwork(nn.Module):
    def __init__(self, input_dim, output_dim, hidden_dim=None, num_blocks=3):
        super(BindNetwork, self).__init__()
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.hidden_dim = hidden_dim or output_dim * 4
        self.num_blocks = num_blocks

        self.proj = nn.Linear(input_dim, output_dim)
        self.blocks = nn.ModuleList([
            nn.ModuleDict({
                "norm": nn.RMSNorm(output_dim),
                "w1": nn.Linear(output_dim, self.hidden_dim),
                "w2": nn.Linear(output_dim, self.hidden_dim),
                "w3": nn.Linear(self.hidden_dim, output_dim)
            })
            for _ in range(num_blocks)
        ])

    def forward(self, image_feature):
        x = self.proj(image_feature)
        for blk in self.blocks:
            h = blk["norm"](x)
            h1 = blk["w1"](h)
            h2 = blk["w2"](h)
            h_mul = h2 * F.silu(h1)
            h3 = blk["w3"](h_mul)
            x = x + h3
        return x

CI = 1024   # ImageBind feature dim
C = 4096    # LLaMA embedding dim

bind_net = BindNetwork(input_dim=CI, output_dim=C)

image_feature = torch.randn(1, CI)
transformed_feature = bind_net(image_feature)
print(transformed_feature.shape)