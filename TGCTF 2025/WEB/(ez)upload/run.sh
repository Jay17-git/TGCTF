#!/bin/sh

# 启动 php-fpm，后台运行
php-fpm -D
# 启动 nginx，不要在后台运行，以保持容器活跃
nginx -g 'daemon off;'