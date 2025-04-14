#!/bin/sh
# 启动 Vite 开发服务器（后台运行）
pnpm dev --host 0.0.0.0 &

echo $FLAG > /tgflagggg
export FLAG="not_flag"
FLAG="not_flag"

httpd -D FOREGROUND

# 保持容器运行（防止退出）
tail -f /dev/null