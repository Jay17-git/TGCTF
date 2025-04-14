FROM python:3.8.7

WORKDIR /app

# 环境变量预配置
ENV FLAG=not_flag

# 系统依赖和权限
RUN pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple && \
    pip config set install.trusted-host https://pypi.tuna.tsinghua.edu.cn && \
    pip install --upgrade pip && \
    pip install pyramid jinja2 && \
    mkdir -p /app/static && \
    chmod 755 /app/static

# 应用文件
COPY app.py .

# 启动脚本
COPY start.sh /start.sh
RUN chmod +x /start.sh

# 入口点配置
CMD ["/start.sh"]