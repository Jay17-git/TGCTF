# 构建阶段
FROM node:18-alpine

# 设置工作目录
WORKDIR /app

# 复制 package.json 和 package-lock.json
COPY package*.json ./
COPY pnpm-lock.yaml ./

# 安装 pnpm
RUN npm install -g pnpm

# 安装依赖
RUN pnpm install

# 复制源代码
COPY . .

# 暴露 5173 端口
EXPOSE 5173


COPY start.sh /start.sh
RUN chmod +x /start.sh

# 直接通过脚本启动
CMD ["/start.sh"]

