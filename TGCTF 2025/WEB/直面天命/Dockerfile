FROM python:3.8-slim
 
COPY . /
WORKDIR /

RUN touch /tgffff11111aaaagggggggg \
    && chmod 755 /tgffff11111aaaagggggggg \
    && chmod 755 /run.sh \
    && pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple \
    && pip install jinja2 -i https://pypi.tuna.tsinghua.edu.cn/simple \
    && pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

EXPOSE 80

ENTRYPOINT ["/run.sh"]