#!/bin/sh
echo $FLAG > /tgfffffllllaagggggg
export FLAG="not_flag"
$FLAG="not_flag"
php-fpm -D
# 启动 nginx，不要在后台运行，以保持容器活跃
nginx -g 'daemon off;'