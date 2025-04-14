import { defineConfig } from "vite"
import vue from "@vitejs/plugin-vue"
import { resolve } from "path"

// https://vitejs.dev/config/
export default defineConfig({
  base: "./",
  plugins: [vue()],
  server: {
    host: '0.0.0.0', // 暴露到外部网络
    // fs: {
    //   allow: ['/']   // 允许访问根目录（高危配置！）
    // },
    allowedHosts: [                        // 允许访问的主机列表
      "node1.tgctf.woooo.tech",
      "node2.tgctf.woooo.tech",           // 添加被拦截的域名
      "all"                               // 保留允许所有主机的兜底配置（可选）
    ],
  },
  resolve: {
    extensions: [".js", ".vue", ".scss", ".ts"],
    alias: [{ find: "@", replacement: resolve(__dirname, "src") }],
  },
})
