# 练习 3-4：自定义配置异常，缺键与类型不对都报同一种错，finally 逐项收尾。
class ConfigError(ValueError):
    pass


def require(config, key):
    if key not in config:
        raise ConfigError(f"missing key: {key}")
    value = config[key]
    if not isinstance(value, int):
        raise ConfigError(f"{key} must be int")
    return value


config = {"width": 640, "height": "480"}
for key in ("width", "height", "fps"):
    try:
        print(key, require(config, key))
    except ConfigError as error:
        print("error:", error)
    finally:
        print("checked:", key)
