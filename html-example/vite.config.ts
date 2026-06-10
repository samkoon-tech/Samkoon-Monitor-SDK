import { defineConfig } from 'vite'

export default defineConfig({
  envDir: '../',
  server: {
    port: 8081,
    host: true,
  },
})
