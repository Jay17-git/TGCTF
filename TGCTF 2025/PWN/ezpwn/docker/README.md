## 1 复制 题目 libc ld到 src文件夹中

## 2 修改dockerfile 中部署程序 复制命令

## 3修改 config 中 xinetd 中的server 文件名和题目文件名相同

## 4 修改 docker-entrypoint 中 patch的参数

构建镜像 推送到docker hub的命令

```
docker build -t username/name .
docker push username/name
```

本地运行的命令

```
docker build -t name .
docker run -d -p 9999:9999 name
nc 127.0.0.1:9999
```

