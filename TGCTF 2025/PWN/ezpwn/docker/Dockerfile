FROM ubuntu:22.04

# 更换镜像源并安装基础依赖
RUN sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
    sed -i 's@//.*security.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
    apt-get update && apt-get -y dist-upgrade && \
    apt-get install -y lib32z1 xinetd vim git python3 python3-pip

# 安装 pwntools 和调试工具
RUN pip3 install --upgrade pip && \
    pip3 install pwntools && \
    apt-get install -y gdb

# 安装 pwndbg
RUN git clone https://github.com/pwndbg/pwndbg /opt/pwndbg && \
    cd /opt/pwndbg && \
    ./setup.sh


# 新建用户并设置工作目录
RUN useradd -m ctf
WORKDIR /home/ctf

# 复制必要的库文件
RUN cp -R /usr/lib* /home/ctf

# 配置特殊设备文件
RUN apt-get install -y fakeroot && \
    mkdir /home/ctf/dev && \
    fakeroot sh -c 'mknod /home/ctf/dev/null c 1 3 && \
    mknod /home/ctf/dev/zero c 1 5 && \
    mknod /home/ctf/dev/random c 1 8 && \
    mknod /home/ctf/dev/urandom c 1 9' && \
    chmod 666 /home/ctf/dev/*

# 设置 chroot 环境下的基础命令
RUN mkdir /home/ctf/bin && \
    cp /bin/sh /home/ctf/bin && \
    cp /bin/ls /home/ctf/bin && \
    cp /bin/cat /home/ctf/bin && \
    cp /usr/bin/timeout /home/ctf/bin

# 部署 xinetd 服务
COPY ./config/ctf.xinetd /etc/xinetd.d/ctf
RUN echo "Blocked by ctf_xinetd" > /etc/banner_fail

# 复制容器启动脚本
COPY ./service/docker-entrypoint.sh /
RUN chmod +x /docker-entrypoint.sh

# 部署漏洞程序和工具
COPY ./src/vuln /home/ctf/vuln
COPY ./tools/* /home/ctf/tools/
RUN chmod +x /home/ctf/vuln

# 设置权限
RUN chown -R root:ctf /home/ctf && \
    chmod -R 770 /home/ctf && \
    touch /home/ctf/flag && \
    chmod 744 /home/ctf/flag

# 指定容器入口点
ENTRYPOINT ["/bin/bash", "/docker-entrypoint.sh"]
