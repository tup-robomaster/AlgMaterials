# 用固定时长模拟生产快于消费时缓冲区溢出与丢帧。


def simulate(produce_ms, consume_ms, buffer_size, duration_ms=1000):
    queue = 0
    produced = 0
    consumed = 0
    dropped = 0
    for t in range(duration_ms):
        if t % produce_ms == 0:
            produced += 1
            if queue < buffer_size:
                queue += 1
            else:
                dropped += 1
        if t % consume_ms == consume_ms // 2 and queue > 0:
            queue -= 1
            consumed += 1
    print(f"缓冲 {buffer_size} 帧，生产 {produce_ms} ms，消费 {consume_ms} ms，"
          f"模拟 {duration_ms} ms：")
    print(f"  产生 {produced} 帧，取走 {consumed} 帧，"
          f"缓冲里剩 {queue} 帧，丢弃 {dropped} 帧")


simulate(10, 12, 5)
simulate(10, 8, 5)
