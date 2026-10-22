#!/usr/bin/env bash
# 在临时目录里复制 7.1 的小工程，按第 2.6 节的分支规范走一遍：
# master → develop → feature → release/v1.0 → hotfix → v1.1。
#
# 这个脚本只用于核对流程，不会改动 7.1 的示例目录，也不联网。
# 运行时所有 Git 配置、时间与身份都固定，仓库建在 mktemp 目录里，退出时删除。

set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
src="$script_dir/../7.1-从空目录到小工程"

work=$(mktemp -d /tmp/other-basics-7.2.XXXXXX) || exit 1
trap 'rm -rf "$work"' EXIT INT TERM

umask 002
chmod 700 "$work"
export HOME="$work/home"
mkdir -p "$HOME"

# 不读系统或用户的 Git 配置；即使误写网络地址也连不出去。
export GIT_CONFIG_NOSYSTEM=1
export GIT_CONFIG_GLOBAL=/dev/null
export GIT_ALLOW_PROTOCOL=file
export GIT_TERMINAL_PROMPT=0
export GIT_CEILING_DIRECTORIES="$work"
export LC_ALL=C
export LANG=C
export GIT_AUTHOR_NAME="练习者"
export GIT_AUTHOR_EMAIL="practice@example.com"
export GIT_COMMITTER_NAME="练习者"
export GIT_COMMITTER_EMAIL="practice@example.com"

gitn=0
stamp() {
    printf '2026-03-14T%02d:%02d:00+00:00' $((9 + $1 / 60)) $(($1 % 60))
}
gc() {
    gitn=$((gitn + 1))
    GIT_AUTHOR_DATE=$(stamp "$gitn") \
    GIT_COMMITTER_DATE=$(stamp "$gitn") git commit "$@"
}
gmerge() {
    gitn=$((gitn + 1))
    GIT_AUTHOR_DATE=$(stamp "$gitn") \
    GIT_COMMITTER_DATE=$(stamp "$gitn") git merge "$@"
}
gtag() {
    GIT_COMMITTER_DATE='2026-03-14T10:00:00+00:00' git tag "$@"
}

mkdir -p "$work/camera-demo"
cp "$src/camera.json" "$src/compute.py" "$src/run.sh" \
   "$src/README.md" "$src/.gitignore" "$work/camera-demo/"
cd "$work/camera-demo"
chmod +x run.sh

# ── 初始提交 ──────────────────────────────────────────
git init -b master >/dev/null
git config user.name "练习者"
git config user.email "practice@example.com"
git add camera.json compute.py run.sh README.md .gitignore
gc -m "feat: 建立相机参数小工程"

git switch -c develop >/dev/null
git switch -c feature/focal-12mm >/dev/null

# ── 功能分支：焦距改为 12 mm ──────────────────────────
sed -i 's/"focal_mm": 8.0/"focal_mm": 12.0/' camera.json
sed -i 's/"fx": 800.0/"fx": 1200.0/' camera.json
sed -i 's/"fy": 800.0/"fy": 1200.0/' camera.json

grep -E '"(focal_mm|fx|fy)"' camera.json
./run.sh > "$work/feature.out"
grep -q '水平视场角：56.14°' "$work/feature.out"
grep -q '垂直视场角：46.21°' "$work/feature.out"
grep -q '焦距折算：1200.0 px' "$work/feature.out"

git add camera.json
gc -m "feat: 试用 12 mm 焦距"
git switch develop >/dev/null
gmerge --no-ff feature/focal-12mm -m "合并功能分支 feature/focal-12mm"
git branch -d feature/focal-12mm

# ── 发布分支：补发布说明，合入 master，打 v1.0 ────────
git switch -c release/v1.0 >/dev/null
cat >> README.md <<'EOF'

## V1.0 发布说明

- 相机 A 的焦距改为 12 mm，`fx` 与 `fy` 同步改为 1200。
- 视场角与数据量由 `compute.py` 重新计算。
EOF
git add README.md
gc -m "docs: 补充 V1.0 发布说明"

git switch master >/dev/null
gmerge --no-ff release/v1.0 -m "合并发布分支 release/v1.0"
gtag -a v1.0 -m "发布 V1.0：12 mm 焦距版本"

git switch develop >/dev/null
gmerge --no-ff release/v1.0 -m "同步发布分支的修改"
git branch -d release/v1.0

# ── 紧急修复：单位改为 MiB/s，合入 master，打 v1.1 ────
git switch master >/dev/null
git switch -c hotfix/unit-label >/dev/null
sed -i 's|MB/s|MiB/s|' compute.py
./run.sh > "$work/hotfix.out"
grep -q '37.50 MiB/s' "$work/hotfix.out"

git add compute.py
gc -m "fix: 数据量单位改为 MiB/s"
git switch master >/dev/null
gmerge --no-ff hotfix/unit-label -m "合并紧急修复 hotfix/unit-label"
gtag -a v1.1 -m "发布 V1.1：数据量单位改为 MiB/s"

git switch develop >/dev/null
gmerge --no-ff hotfix/unit-label -m "同步紧急修复"

# ── 删除已合并的紧急修复，核对最终状态 ────────────────
printf '\n── 删除分支前的历史 ──\n'
git log --graph --oneline --decorate --all --no-color

git branch -d hotfix/unit-label

printf '\n── 最终状态 ──\n'
git branch
git tag -n
printf '工作区：'
if [ -z "$(git status --porcelain)" ]; then
    printf '干净\n'
else
    printf '有未提交的改动\n'
    git status --short
    exit 1
fi

[ "$(git branch --format='%(refname:short)' | sort)" = "develop
master" ] || { echo "分支不对"; exit 1; }
[ "$(git tag --list | sort)" = "v1.0
v1.1" ] || { echo "标签不对"; exit 1; }
[ "$(git rev-parse master^{tree})" = "$(git rev-parse develop^{tree})" ] \
    || { echo "master 与 develop 内容不一致"; exit 1; }
git merge-base --is-ancestor v1.0 master
git merge-base --is-ancestor v1.1 master
[ "$(git cat-file -t v1.0)" = "tag" ] || { echo "v1.0 不是附注标签"; exit 1; }
[ "$(git cat-file -t v1.1)" = "tag" ] || { echo "v1.1 不是附注标签"; exit 1; }

printf '7.2 分支流程检查通过\n'
