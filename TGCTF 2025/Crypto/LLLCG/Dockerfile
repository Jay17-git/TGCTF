FROM python:3.10.15-slim
COPY challenge.py /
COPY secret.py /
COPY sever.py /
RUN sed -i s@/deb.debian.org/@/mirrors.aliyun.com/@g /etc/apt/sources.list.d/debian.sources && \
    apt-get update -y && \
    apt-get install socat -y && \
    pip install pycryptodome -i https://pypi.tuna.tsinghua.edu.cn/simple/


EXPOSE 10001
ENTRYPOINT ["python", "-u", "/sever.py"]
