<template>
  <div id="demo-wrapper">
    <nav class="navbar">
      <div class="logo">Samkoon Monitor SDK Integration (Vue)</div>
      <button v-if="initialized" id="btn-back" @click="reset">Back</button>
    </nav>

    <div class="main-container" id="content-node">
      <!-- 1. 配置阶段 -->
      <section v-if="!initialized" class="login-card">
        <div id="enterprise-form">
          <div class="form-item">
            <label>Client ID</label>
            <input v-model="config.clientId" placeholder="Enter Client ID" />
          </div>
          <div class="form-item">
            <label>Secret Key</label>
            <input v-model="config.secretKey" type="password" placeholder="Enter Secret Key" />
          </div>
        </div>
        <div class="form-item">
          <label>Instance ID (View ID)</label>
          <input v-model="config.instanceId" placeholder="Enter Instance ID" />
        </div>
        <button class="btn-init" @click="initSDK">Initialize & Login</button>
      </section>

      <!-- 2. SDK 运行阶段 -->
      <section v-else id="sdk-viewport">
        <samkoon-monitor-sdk
          style="display: block; width: 100%; height: 100%"
          :instance-id="config.instanceId"
          :client-id="config.clientId"
          :secret-key="config.secretKey"
        />
      </section>
    </div>
  </div>
</template>

<script>
/**
 * Vue 2.x/3.x 接入完整的 SDK 示例 (使用 Web Component 模式)
 */
export default {
  name: 'VueSDKDemo',
  data() {
    return {
      initialized: false,
      config: {
        clientId: import.meta.env.VITE_SDK_CLIENT_ID || '',
        secretKey: import.meta.env.VITE_SDK_SECRET_KEY || '',
        instanceId: import.meta.env.VITE_SDK_INSTANCE_ID || '',
      },
    }
  },
  methods: {
    initSDK() {
      if (!this.config.clientId || !this.config.secretKey || !this.config.instanceId) {
        alert('Missing configuration')
        return
      }
      this.initialized = true
    },
    reset() {
      this.initialized = false
    },
  },
}
</script>

<style>
:root {
  --primary-color: #2563eb;
  --primary-hover: #1d4ed8;
  --bg-color: #f8fafc;
  --card-bg: #ffffff;
  --text-main: #1e293b;
  --text-secondary: #64748b;
}

body {
  margin: 0;
  padding: 0;
}

#demo-wrapper {
  height: 100vh;
  display: flex;
  flex-direction: column;
  background-image: url('/bk.png');
  background-size: cover;
  background-position: center;
  background-repeat: no-repeat;
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

.navbar {
  background: rgba(255, 255, 255, 0.7);
  backdrop-filter: blur(12px);
  border-bottom: 1px solid rgba(226, 232, 240, 0.3);
  padding: 12px 24px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  position: sticky;
  top: 0;
  z-index: 100;
}

.logo {
  font-weight: 700;
  font-size: 1.1rem;
  background: linear-gradient(135deg, #2563eb, #7c3aed);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.main-container {
  flex: 1;
  display: flex;
  justify-content: center;
  align-items: center;
  position: relative;
}

.login-card {
  background: rgba(255, 255, 255, 0.9);
  backdrop-filter: blur(10px);
  width: 100%;
  max-width: 420px;
  padding: 32px;
  border-radius: 16px;
  box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.1), 0 8px 10px -6px rgba(0, 0, 0, 0.1);
  z-index: 10;
}

.form-item {
  margin-bottom: 16px;
}
.form-item label {
  display: block;
  margin-bottom: 6px;
  font-weight: 600;
  color: var(--text-main);
  font-size: 0.875rem;
}

.form-item input {
  width: 100%;
  padding: 10px 12px;
  border: 1px solid #e2e8f0;
  border-radius: 8px;
  box-sizing: border-box;
  font-size: 0.95rem;
  transition: border-color 0.2s;
}

.form-item input:focus {
  outline: none;
  border-color: var(--primary-color);
}

.btn-init {
  width: 100%;
  padding: 12px;
  background: var(--primary-color);
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  font-size: 1rem;
  font-weight: 600;
  margin-top: 8px;
  transition: background 0.2s;
}

.btn-init:hover {
  background: var(--primary-hover);
}

#btn-back {
  padding: 6px 12px;
  background: #f1f5f9;
  border: 1px solid #e2e8f0;
  border-radius: 6px;
  cursor: pointer;
  font-weight: 500;
}

#sdk-viewport {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: #000;
}
</style>
