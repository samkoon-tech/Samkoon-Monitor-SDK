#!/usr/bin/env bash
# playground push 后自动触发 Codeup Qt5 流水线
# 由 git core.hooksPath 引用
#
# 安装：git config core.hooksPath scripts/hooks
# 或一次性：cd playground && git config core.hooksPath scripts/hooks

set -e

# 仅在推送到 master / main 时触发
while read -r local_ref local_sha remote_ref remote_sha; do
  branch="${remote_ref#refs/heads/}"
  if [[ "$branch" == "master" || "$branch" == "main" ]]; then
    echo "🔔 推送到 ${branch}，触发 Qt5 Windows 打包流水线"
    "$(git rev-parse --show-toplevel)/../../bin/build-qt5.sh" --branch "$branch" || \
      echo "⚠️  触发失败（不影响 push），可手动执行：bin/build-qt5.sh"
  fi
done