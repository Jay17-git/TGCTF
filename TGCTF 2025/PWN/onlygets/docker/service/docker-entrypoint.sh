#!/bin/sh

# Get the user
user=$(ls /home)

# Check the environment variables for the flag and assign to INSERT_FLAG
if [ "$DASFLAG" ]; then
    INSERT_FLAG="$DASFLAG"
    export DASFLAG=no_FLAG
    DASFLAG=no_FLAG
elif [ "$FLAG" ]; then
    INSERT_FLAG="$FLAG"
    export FLAG=no_FLAG
    FLAG=no_FLAG
elif [ "$GZCTF_FLAG" ]; then
    INSERT_FLAG="$GZCTF_FLAG"
    export GZCTF_FLAG=no_FLAG
    GZCTF_FLAG=no_FLAG
else
    INSERT_FLAG="flag{TEST_Dynamic_FLAG}"
fi

# 将FLAG写入文件 请根据需要修改
echo $INSERT_FLAG | tee /home/$user/flag

# 赋予程序运行权限
chmod 777 /home/ctf/vuln
chmod 777 /home/ctf/TGCTF.so

# 调试信息
echo "=== 调试信息 ==="
echo "当前用户: $(whoami)"
echo "当前目录: $(pwd)"
echo "文件列表:"
ls -la /home/ctf/
echo "文件权限:"
ls -l /home/ctf/TGCTF.so
echo "文件类型:"
file /home/ctf/TGCTF.so
echo "依赖检查:"
ldd /home/ctf/TGCTF.so || echo "ldd检查失败"

# 设置环境变量
export LD_PRELOAD=/TGCTF.so
echo "LD_PRELOAD设置为: $LD_PRELOAD"

# 启动服务
echo "启动xinetd..."
/etc/init.d/xinetd start;
sleep infinity;