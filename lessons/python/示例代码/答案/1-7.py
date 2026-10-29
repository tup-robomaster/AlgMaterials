# 练习 1-7：合并默认配置与覆盖配置，打印改动过的键。
defaults = {"exposure": 10, "gain": 1.0}
override = {"gain": 2.0, "gamma": 1.2}
config = defaults | override
print(sorted(config.items()))
for key in sorted(config):
    print(f"{key}={config[key]}")
print("changed:", sorted(set(defaults) & set(override)))
