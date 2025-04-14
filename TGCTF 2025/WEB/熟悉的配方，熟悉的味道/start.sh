#!/bin/sh

# 写入flag（确保目录存在）
if [ -n "$FLAG" ]; then
    echo "$FLAG" > /flagggggg_tgctf2025_asjdklalkcnkjassjhdlk
    chmod 644 /flagggggg_tgctf2025_asjdklalkcnkjassjhdlk
else
    echo "Warning: FLAG environment variable not set!" >&2
fi

# 安全擦除环境变量
unset FLAG
export FLAG="not_flag"

# 启动应用
exec python app.py