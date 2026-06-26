import { defineConfig } from 'vite'

export default defineConfig({
  envDir: '../',
  server: {
    port: 8081,
    host: true,
    headers: {
      // VNC 组件连接本地插件需要 Private Network Access 权限
      'Permissions-Policy': 'local-network-access=*',
    },
  },
})
