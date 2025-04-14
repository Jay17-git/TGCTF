FROM python:3.12.5-bookworm

# 创建用户和组
RUN addgroup --gid 404 appuser && \
    adduser --uid 404 --gid 404 --disabled-password --no-create-home appuser

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY templates templates
COPY *.py .

# 更改文件所有者
RUN chown -R 404:404 /app

USER 404:404

CMD ["gunicorn", "--workers", "4", "--bind", "0.0.0.0:9045", "app:app"]