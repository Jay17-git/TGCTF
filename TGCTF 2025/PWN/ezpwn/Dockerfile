# 使用基础镜像将该位置的20.04替换成你想要的版本
FROM ubuntu:22.04
 
# 更新 apt 包索引并安装所需的软件，异架构安装软件办法有所不同，请自行解决
RUN apt-get update && apt-get install -y \
    socat \
    gdb \
    gdbserver \
    vim \
    gcc
 
# 在 /tmp 下生成一个名为 run.sh 的脚本
RUN echo 'socat TCP-LISTEN:9999,fork,reuseaddr EXEC:./pwn' > /tmp/run.sh
 
# 赋予脚本可执行权限
RUN chmod +x /tmp/run.sh
 
# 暴露容器的 9999 和 10000 端口
EXPOSE 9999 10000
 
# 定义容器启动时执行的命令
CMD ["/bin/bash"]