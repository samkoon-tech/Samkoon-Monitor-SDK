import React, { useState } from 'react'
import './App.css'

/**
 * React 完整接入工程示例
 */
const ReactApp: React.FC = () => {
  const [clientId, setClientId] = useState(import.meta.env.VITE_SDK_CLIENT_ID || '')
  const [secretKey, setSecretKey] = useState(import.meta.env.VITE_SDK_SECRET_KEY || '')
  const [instanceId, setInstanceId] = useState(import.meta.env.VITE_SDK_INSTANCE_ID || '')
  const [isInit, setIsInit] = useState(false)
  const [navCollapsed, setNavCollapsed] = useState(false)

  const initSDK = () => {
    if (!clientId || !secretKey || !instanceId) {
      alert('Missing configuration')
      return
    }
    setIsInit(true)
  }

  const reset = () => {
    setIsInit(false)
    setNavCollapsed(false)
  }

  return (
    <div id="demo-wrapper" className={isInit ? 'active' : ''}>
      <nav className={`navbar${navCollapsed ? ' collapsed' : ''}`} id="navbar">
        <div className="logo">Samkoon Monitor SDK Integration (React)</div>
        <div className="navbar-actions">
          {isInit && (
            <button id="btn-back" onClick={reset}>
              Back
            </button>
          )}
          <button
            id="btn-navbar-toggle"
            title="折叠导航栏"
            onClick={() => setNavCollapsed(true)}
          >
            ▲
          </button>
        </div>
      </nav>

      {navCollapsed && (
        <button
          id="navbar-restore"
          title="展开导航栏"
          onClick={() => setNavCollapsed(false)}
        >
          ▼ 导航
        </button>
      )}

      <div className="main-container" id="content-node">
        {!isInit ? (
          <div className="login-card">
            <div id="enterprise-form">
              <div className="form-item">
                <label>Client ID</label>
                <input
                  type="text"
                  value={clientId}
                  onChange={(e) => setClientId(e.target.value)}
                  placeholder="Enter Client ID"
                />
              </div>
              <div className="form-item">
                <label>Secret Key</label>
                <input
                  type="password"
                  value={secretKey}
                  onChange={(e) => setSecretKey(e.target.value)}
                  placeholder="Enter Secret Key"
                />
              </div>
            </div>

            <div className="form-item">
              <label>Instance ID (View ID)</label>
              <input
                type="text"
                value={instanceId}
                onChange={(e) => setInstanceId(e.target.value)}
                placeholder="Enter Instance ID"
              />
            </div>

            <button className="btn-init" onClick={initSDK}>
              Initialize & Login
            </button>
          </div>
        ) : (
          <div id="sdk-viewport">
            <samkoon-monitor-sdk
              style={{ display: 'block', width: '100%', height: '100%' }}
              instance-id={instanceId}
              client-id={clientId}
              secret-key={secretKey}
            />
          </div>
        )}
      </div>
    </div>
  )
}

export default ReactApp
