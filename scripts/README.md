# Playground CI 脚本

## 触发 Qt5 Windows 打包流水线

```bash
# 安装 flowctl
brew install flowctl
# 或下载二进制：https://codeup-pipeline.oss-cn-hangzhou.aliyuncs.com/flowctl/latest/

# 配置凭证（二选一）
export FLOWCTL_AK=<your-ak>
export FLOWCTL_SK=<your-sk>
# 或：flowctl config set --access-key xxx --access-secret yyy

# 手动触发构建
bin/build-qt5.sh

# 指定分支
bin/build-qt5.sh --branch dev

# 查询最近一次运行状态
bin/build-qt5.sh --status
```

## push 自动触发

```bash
# 在 playground 仓库下启用
cd playground
git config core.hooksPath scripts/hooks
```

之后 push 到 master / main 时会自动调用 `bin/build-qt5.sh` 触发流水线，
失败不影响 push 完成（仅 echo 警告）。

## 流水线地址

`https://codeup.aliyun.com/samkoon/f-end/samkoon-monitor-sdk-playground/-/pipelines`

构建产物下载：流水线详情页 → "产物" Tab → `SamkoonMonitorQt5-windows-x64.zip`