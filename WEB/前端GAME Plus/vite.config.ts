import { defineConfig } from "vite"
import vue from "@vitejs/plugin-vue"
import { resolve } from "path"

// https://vitejs.dev/config/
export default defineConfig({
  base: "./",
  plugins: [vue()],
  server: {
    host: '0.0.0.0', 
    allowedHosts: [                       
      "node1.tgctf.woooo.tech",
      "node2.tgctf.woooo.tech",          
      "all"                              
    ],
  },
  resolve: {
    extensions: [".js", ".vue", ".scss", ".ts"],
    alias: [{ find: "@", replacement: resolve(__dirname, "src") }],
  },
})
