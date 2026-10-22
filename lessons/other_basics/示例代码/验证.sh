#!/bin/bash
# 把第 1–7 部分用到的示例目录复制到临时位置，按课里的顺序跑一遍，
# 把实际结果和课里写的预期结果逐项对照。
#
# 用法（在课程页那一层，也就是 示例代码/ 的上一层）：
#     bash 示例代码/验证.sh
#
# 也可以先 cd 到 示例代码/，再：
#     bash 验证.sh
#
# 这份脚本只读课程里的示例，所有改动都发生在 /tmp 下的副本里；
# 跑完会把副本删掉。它不联网，也不需要 sudo。

set -u

cd "$(dirname "$0")" || exit 1

# 固定名字的 /tmp 目录在多人机器上可能被抢先创建；改用 mktemp，退出时清掉。
work=$(mktemp -d /tmp/other-basics-verify.XXXXXX) || exit 1
trap 'rm -rf "$work"' EXIT INT TERM

# 1.4 的权限检查要看到 664/775，所以 umask 保持 002；
# 临时目录本身收紧到 700，同机其他用户进不来。
umask 002
chmod 700 "$work"
mkdir -p "$work/示例代码"
cp -r 1.* 2.* 3.* 4.* 5.* 6.* 7.* "$work/示例代码"/ || exit 1

# Git 练习不读用户自己的全局配置，也不碰 ~/.ssh。
# 身份、时间和语言都固定下来，输出才稳定。
# 只允许 file 协议：即使某条命令误指网络地址，也连不出去。
export HOME="$work/home"
mkdir -p "$HOME"
export GIT_CONFIG_NOSYSTEM=1
export GIT_CONFIG_GLOBAL=/dev/null
export GIT_ALLOW_PROTOCOL=file
export GIT_TERMINAL_PROMPT=0
export GIT_CEILING_DIRECTORIES="$work"
export LC_ALL=C
export PYTHONIOENCODING=utf-8
export PYTHONNOUSERSITE=1
unset HTTP_PROXY HTTPS_PROXY ALL_PROXY NO_PROXY \
      http_proxy https_proxy all_proxy no_proxy PYTHONPATH
export GIT_AUTHOR_NAME=alice
export GIT_AUTHOR_EMAIL=alice@example.com
export GIT_COMMITTER_NAME=alice
export GIT_COMMITTER_EMAIL=alice@example.com

gitn=0
stamp() {
    printf '2026-03-14T%02d:%02d:00+00:00' $((9 + $1 / 60)) $(($1 % 60))
}
gc() {
    gitn=$((gitn + 1))
    GIT_AUTHOR_DATE=$(stamp "$gitn") GIT_COMMITTER_DATE=$(stamp "$gitn") git commit "$@"
}
gmerge() {
    gitn=$((gitn + 1))
    GIT_AUTHOR_DATE=$(stamp "$gitn") GIT_COMMITTER_DATE=$(stamp "$gitn") git merge "$@"
}

pass=0
fail=0

check() {
    name=$1
    want=$2
    got=$3
    if [ "$got" = "$want" ]; then
        printf '  [通过] %s\n' "$name"
        pass=$((pass + 1))
    else
        printf '  [不对] %s\n' "$name"
        printf '    预期：\n%s\n' "$want" | sed 's/^/      /'
        printf '    实际：\n%s\n' "$got" | sed 's/^/      /'
        fail=$((fail + 1))
    fi
}

printf '════════════════════════════════════════════\n'
printf ' 其他基础 · 第 1–7 部分示例自检\n'
printf '════════════════════════════════════════════\n'

# ── 1.1 目录结构 ──────────────────────────────────────
printf '\n1.1 目录结构\n'
c1="$work/示例代码/1.1-目录与路径"
check "当前目录里有一个 demo 目录" "demo" "$(ls -1 "$c1")"
check "demo 里有一个 note.txt" "note.txt" "$(ls -1 "$c1/demo")"
check "note.txt 的第一行" "Linux 目录结构示例" "$(head -n 1 "$c1/demo/note.txt")"

# ── 1.2 路径与文件操作 ────────────────────────────────
printf '\n1.2 路径与文件操作\n'
c2="$work/示例代码/1.2-路径与文件"
mkdir "$c2/backup"
cp "$c2/notes.txt" "$c2/backup/"
cp -r "$c2/data" "$c2/backup/"
mv "$c2/backup/notes.txt" "$c2/backup/notes-copy.txt"
check "复制与改名之后 backup 里的内容" "data
notes-copy.txt" "$(ls -1 "$c2/backup")"
rm "$c2/backup/notes-copy.txt"
rm -r "$c2/backup"
check "删除 backup 之后只剩两个条目" "data
notes.txt" "$(ls -1 "$c2")"
ln -s notes.txt "$c2/link.txt"
check "符号链接指向 notes.txt" "notes.txt" "$(readlink "$c2/link.txt")"
check "通过链接读到同一份内容" "$(cat "$c2/notes.txt")" "$(cat "$c2/link.txt")"
rm "$c2/link.txt"
check "删除链接后目标文件还在" "data
notes.txt" "$(ls -1 "$c2")"
check "删除链接后目标文件仍可读" "第一行：路径与文件操作
第二行：复制、移动、删除
第三行：隐藏文件与通配符" "$(cat "$c2/notes.txt")"
echo "带空格的文件名" > "$c2/my note.txt"
check "带空格的文件名可以读出来" "带空格的文件名" "$(cat "$c2/my note.txt")"
check "普通列表里能看到带空格的文件" "data
my note.txt
notes.txt" "$(ls -1 "$c2")"
touch "$c2/.hidden"
check "普通列表看不见隐藏文件" "data
my note.txt
notes.txt" "$(ls -1 "$c2")"
check "加 -a 才看得见隐藏文件" ".hidden" "$(ls -1a "$c2" | grep '^\.hidden$')"
touch "$c2/a1.txt" "$c2/a2.txt" "$c2/b1.txt"
check "通配符 a*.txt 匹配两个" "a1.txt
a2.txt" "$(ls -1 "$c2"/a*.txt | sed "s#^$c2/##")"
check "通配符 ?1.txt 匹配两个" "a1.txt
b1.txt" "$(ls -1 "$c2"/?1.txt | sed "s#^$c2/##")"
rm "$c2/a1.txt" "$c2/a2.txt" "$c2/b1.txt"

# ── 1.3 查看、搜索与统计 ──────────────────────────────
printf '\n1.3 查看、搜索与统计\n'
c3="$work/示例代码/1.3-查看与搜索"
check "scores.txt 一共 5 行" "5" "$(wc -l < "$c3/scores.txt")"
check "苹果出现 2 次" "2" "$(grep -c 苹果 "$c3/scores.txt")"
check "按行号找到苹果" "1:苹果 3
3:苹果 2" "$(grep -n 苹果 "$c3/scores.txt")"
check "两行日志里都有启动服务" "2" "$(grep -rc 启动 "$c3/logs" | awk -F: '{s += $2} END {print s}')"
check "names.txt 去重后有 3 种" "3" "$(sort "$c3/names.txt" | uniq | wc -l)"
check "词频统计" "      2 apple
      3 banana
      1 orange" "$(sort "$c3/names.txt" | uniq -c)"

# ── 1.4 权限 ──────────────────────────────────────────
printf '\n1.4 权限\n'
c4="$work/示例代码/1.4-权限"
check "初始权限是 664" "-rw-rw-r-- 664" "$(stat -c '%A %a' "$c4/hello.sh")"
check "没有执行权限时不能直接运行" "126" "$("$c4/hello.sh" >/dev/null 2>&1; echo $?)"
chmod +x "$c4/hello.sh"
check "加执行权限后是 775" "-rwxrwxr-x 775" "$(stat -c '%A %a' "$c4/hello.sh")"
check "脚本能运行并打印当前目录" "脚本开始运行
当前目录：$c4" "$(cd "$c4" && ./hello.sh)"
chmod 644 "$c4/hello.sh"
chmod u+x "$c4/hello.sh"
check "只给所有者加执行权限后是 744" "-rwxr--r-- 744" "$(stat -c '%A %a' "$c4/hello.sh")"
chmod 644 "$c4/hello.sh"
check "八进制写法改成 644" "-rw-r--r-- 644" "$(stat -c '%A %a' "$c4/hello.sh")"
chmod 600 "$c4/data.txt"
check "数据文件改成 600" "-rw------- 600" "$(stat -c '%A %a' "$c4/data.txt")"

# ── 1.5 进程与信号 ────────────────────────────────────
printf '\n1.5 进程与信号\n'
check "信号 2 的名字" "INT" "$(kill -l 2)"
check "信号 15 的名字" "TERM" "$(kill -l 15)"
check "信号 9 的名字" "KILL" "$(kill -l 9)"
c5="$work/示例代码/1.5-进程"
chmod +x "$c5/count.sh"
check "count.sh 的前两行" "计数：1
计数：2" "$(timeout 2 "$c5/count.sh" | head -n 2)"

# ── 1.6 管道与重定向 ──────────────────────────────────
printf '\n1.6 管道与重定向\n'
c6="$work/示例代码/1.6-管道与重定向"
check "排序后去重" "apple
banana
orange" "$(sort "$c6/words.txt" | uniq)"
check "重定向写入三行" "3" "$(grep banana "$c6/words.txt" > "$c6/banana.txt"; wc -l < "$c6/banana.txt")"
check "追加一行后变成四行" "4" "$(echo grape >> "$c6/banana.txt"; wc -l < "$c6/banana.txt")"
check "输入重定向不显示文件名" "6" "$(wc -l < "$c6/words.txt")"
check "错误流可以单独接住" "1" "$(LC_ALL=C ls "$c6/no-such-file" 2> "$c6/err.txt"; wc -l < "$c6/err.txt")"
check "失败命令的退出码是 1" "1" "$(false; echo $?)"
check "成功命令的退出码是 0" "0" "$(true; echo $?)"
check "命令替换的结果" "今天是 星期五" "$(echo "今天是 $(echo 星期五)")"

# ── 1.7 压缩与远程 ────────────────────────────────────
printf '\n1.7 压缩与远程\n'
c7="$work/示例代码/1.7-压缩与远程"
(
    cd "$c7" || exit 1
    tar -czf project.tar.gz project
    mkdir unpack
    tar -xzf project.tar.gz -C unpack
)
check "tar 解包后的文件" "unpack/project/README.md
unpack/project/src/main.py" "$(cd "$c7" && find unpack -type f | sort)"
check "tar 列表里有两份文件" "2" "$(cd "$c7" && tar -tzf project.tar.gz | grep -c '\.md\|\.py')"
check "rsync 能报出版本" "rsync" "$(rsync --version | head -n 1 | awk '{print $1}')"

# ── 2.1 仓库、暂存区与提交 ────────────────────────────
printf '\n2.1 仓库、暂存区与提交\n'
c21="$work/示例代码/2.1-第一次提交"
cd "$c21" || exit 1
git init -q -b main
git config user.name "alice"
git config user.email "alice@example.com"
git add README.md
gc -m "添加 README" >/dev/null
git add average.py
gc -m "添加平均分计算脚本" >/dev/null
echo 'print("计算完成")' >> average.py
git add average.py
gc -m "脚本结束时打印提示" >/dev/null
check "2.1 一共三次提交" "3" "$(git rev-list --count HEAD)"
check "2.1 最近一次提交信息" "脚本结束时打印提示" "$(git log -1 --format=%s)"
check "2.1 工作区干净" "" "$(git status --porcelain)"
check "2.1 暂存区为空" "" "$(git diff --staged)"
check "2.1 脚本最后一行" 'print("计算完成")' "$(tail -n 1 average.py)"
cd "$work/示例代码" || exit 1

# ── 2.2 分支与合并 ────────────────────────────────────
printf '\n2.2 分支与合并\n'
c22="$work/示例代码/2.2-分支与合并"
cd "$c22" || exit 1
git init -q -b main
git config user.name "alice"
git config user.email "alice@example.com"
git add README.md report.py
gc -m "初始版本：输出报告标题" >/dev/null
git switch -q -c feature-title
echo 'print("2026 春季学期")' >> report.py
git add report.py
gc -m "在标题后加上学期" >/dev/null
git switch -q main
git merge --no-edit feature-title >/dev/null
git branch -d feature-title >/dev/null
git switch -q -c feature-note
printf '\n说明：分数为百分制。\n' >> README.md
git add README.md
gc -m "补充分数说明" >/dev/null
git switch -q main
echo 'print("共 3 名学生")' >> report.py
git add report.py
gc -m "输出学生人数" >/dev/null
gmerge --no-edit feature-note -m "合并：补充分数说明" >/dev/null
git branch -d feature-note >/dev/null
check "2.2 一共五次提交" "5" "$(git rev-list --count HEAD)"
check "2.2 有一次合并提交" "合并：补充分数说明" "$(git log --merges --format=%s)"
check "2.2 只剩主分支" "main" "$(git branch --format='%(refname:short)')"
check "2.2 工作区干净" "" "$(git status --porcelain)"
check "2.2 报告脚本的三行" "print(\"成绩报告\")
print(\"2026 春季学期\")
print(\"共 3 名学生\")" "$(cat report.py)"
check "2.2 说明里补了一行" "1" "$(grep -c '说明：分数为百分制。' README.md)"
cd "$work/示例代码" || exit 1

# ── 2.3 远程仓库与 SSH ────────────────────────────────
printf '\n2.3 远程仓库与 SSH\n'
c23="$work/示例代码/2.3-远程仓库"
cd "$c23" || exit 1
git init -q --bare -b main remote.git
git clone -q remote.git work
cd work
git config user.name "alice"
git config user.email "alice@example.com"
cp ../项目文件/README.md .
cp ../项目文件/score.py .
git add README.md score.py
gc -m "初始提交：计分工具" >/dev/null
git push -q -u origin main
cd ..
git clone -q remote.git other
cd other
git config user.name "bob"
git config user.email "bob@example.com"
printf '\n用法：python3 score.py\n' >> README.md
git add README.md
gc -m "补充用法说明" >/dev/null
git push -q
cd ../work
git fetch -q
git pull -q --ff-only
check "2.3 远端是裸仓库" "true" "$(git -C "$c23/remote.git" rev-parse --is-bare-repository)"
check "2.3 工作副本有两次提交" "2" "$(git rev-list --count HEAD)"
check "2.3 最近一次提交信息" "补充用法说明" "$(git log -1 --format=%s)"
check "2.3 拉到了对方的改动" "1" "$(grep -c '用法：python3 score.py' README.md)"
check "2.3 工作区干净" "" "$(git status --porcelain)"
check "2.3 远程名是 origin" "origin" "$(git remote)"
check "2.3 上游是 origin/main" "origin/main" "$(git rev-parse --abbrev-ref --symbolic-full-name '@{u}')"
check "2.3 远端主分支有两次提交" "2" "$(git -C "$c23/remote.git" rev-list --count main)"
cd "$c23" || exit 1
git init -q -b main demo
cd demo
git remote add origin git@github.com:用户名/仓库名.git
check "2.3 新仓库能添加 origin" "origin" "$(git remote)"
check "2.3 新仓库的 origin 地址" "git@github.com:用户名/仓库名.git" "$(git remote get-url origin)"
cd "$work/示例代码" || exit 1

# ── 2.4 冲突、回退与撤销 ──────────────────────────────
printf '\n2.4 冲突、回退与撤销\n'
c24="$work/示例代码/2.4-冲突与回退"
cd "$c24" || exit 1
git init -q -b main
git config user.name "alice"
git config user.email "alice@example.com"
git add config.txt
gc -m "添加超时与重试配置" >/dev/null
git switch -q -c feature-timeout
cat > config.txt <<'EOF'
timeout = 30
retries = 3
EOF
git add config.txt
gc -m "把超时改为 30 秒" >/dev/null
git switch -q main
cat > config.txt <<'EOF'
timeout = 20
retries = 3
EOF
git add config.txt
gc -m "把超时改为 20 秒" >/dev/null
git merge feature-timeout >/dev/null 2>&1 || true
check "2.4 冲突文件里有 HEAD 标记" "1" "$(grep -c '^<<<<<<< HEAD$' config.txt)"
check "2.4 状态里是未合并" "UU config.txt" "$(git status --short)"
cat > config.txt <<'EOF'
timeout = 25
retries = 3
EOF
git add config.txt
gc -m "合并两个超时改动，取 25 秒" >/dev/null
check "2.4 合并后没有冲突标记" "0" "$(grep -c '<<<<<<<' config.txt)"
check "2.4 合并后的两行" "timeout = 25
retries = 3" "$(cat config.txt)"
check "2.4 合并提交有一次" "合并两个超时改动，取 25 秒" "$(git log --merges --format=%s)"

echo 'timeout = 99' > config.txt
check "2.4 restore 前是工作区改动" " M config.txt" "$(git status --short)"
git restore config.txt
check "2.4 restore 恢复成提交内容" "timeout = 25
retries = 3" "$(cat config.txt)"
echo 'timeout = 99' > config.txt
git add config.txt
check "2.4 暂存后的状态" "M  config.txt" "$(git status --short)"
git restore --staged config.txt
check "2.4 取消暂存后仍是工作区改动" " M config.txt" "$(git status --short)"
git restore config.txt

echo 'debug = false' >> config.txt
git add config.txt
gc -m "添加 debug 开关" >/dev/null
check "2.4 soft 前最近一次提交" "添加 debug 开关" "$(git log -1 --format=%s)"
git reset --soft HEAD~1
check "2.4 soft 退回上一次提交" "合并两个超时改动，取 25 秒" "$(git log -1 --format=%s)"
check "2.4 soft 保住暂存区" "M  config.txt" "$(git status --short)"
gc -m "添加 debug 开关" >/dev/null
git reset HEAD~1 >/dev/null
check "2.4 mixed 取消暂存" " M config.txt" "$(git status --short)"
git add config.txt
gc -m "添加 debug 开关" >/dev/null
git reset --hard HEAD~1 >/dev/null
check "2.4 hard 退回上一次提交" "合并两个超时改动，取 25 秒" "$(git log -1 --format=%s)"
check "2.4 hard 后工作区干净" "" "$(git status --porcelain)"
check "2.4 hard 丢掉了 debug 行" "0" "$(grep -c debug config.txt)"

echo 'debug = false' >> config.txt
git add config.txt
gc -m "添加 debug 开关" >/dev/null
gitn=$((gitn + 1))
GIT_AUTHOR_DATE=$(stamp "$gitn") GIT_COMMITTER_DATE=$(stamp "$gitn") git revert HEAD --no-edit >/dev/null
check "2.4 revert 生成了反向提交" 'Revert "添加 debug 开关"' "$(git log -1 --format=%s)"
check "2.4 revert 后没有 debug 行" "0" "$(grep -c debug config.txt)"

echo 'timeout = 99' > config.txt
git stash >/dev/null
check "2.4 stash 后工作区干净" "" "$(git status --porcelain)"
check "2.4 stash 后文件恢复" "timeout = 25
retries = 3" "$(cat config.txt)"
check "2.4 stash 列表有一条" "1" "$(git stash list | wc -l)"
git stash pop >/dev/null 2>&1
check "2.4 stash pop 后改动回来" " M config.txt" "$(git status --short)"
check "2.4 stash pop 后文件是 99" "timeout = 99" "$(cat config.txt)"
git restore config.txt
cd "$work/示例代码" || exit 1

# ── 2.5 提交信息规范 ──────────────────────────────────
printf '\n2.5 提交信息规范\n'
c25="$work/示例代码/2.5-提交信息"
cd "$c25" || exit 1
git init -q -b main
git config user.name "alice"
git config user.email "alice@example.com"
git add README.md score.py
gc -m "添加计分工具" -m "读取固定的三个分数，输出总分。" >/dev/null
cat > score.py <<'EOF'
scores = [12, 7, 9]
print("总分：", sum(scores))
EOF
git add score.py
gc -m "fix: 输出中加上总分说明" -m "原来只打印数字，看不出含义；在数字前加上“总分：”。" >/dev/null
check "2.5 最近一次标题" "fix: 输出中加上总分说明" "$(git log -1 --format=%s)"
check "2.5 最近一次正文" "fix: 输出中加上总分说明

原来只打印数字，看不出含义；在数字前加上“总分：”。" "$(git log -1 --format=%B)"
check "2.5 脚本里出现总分" "1" "$(grep -c '总分' score.py)"
check "2.5 一共两次提交" "2" "$(git rev-list --count HEAD)"
gc --amend -m "fix: 输出中加上总分说明" -m "原来只打印数字，看不出含义；现在在数字前加上“总分：”，并保留原有计算。" >/dev/null
check "2.5 amend 后正文更新" "fix: 输出中加上总分说明

原来只打印数字，看不出含义；现在在数字前加上“总分：”，并保留原有计算。" "$(git log -1 --format=%B)"
check "2.5 工作区干净" "" "$(git status --porcelain)"
cd "$work/示例代码" || exit 1

# ── 2.6 分支规范与 .gitignore ─────────────────────────
printf '\n2.6 分支规范与 .gitignore\n'
c26="$work/示例代码/2.6-分支规范与忽略"
cd "$c26" || exit 1
git init -q -b main
git config user.name "alice"
git config user.email "alice@example.com"
cat > .gitignore <<'EOF'
# 构建产物
build/

# 日志
logs/

# 模型权重
weights/

# 本地配置与密钥
config.local.yaml
*.key
EOF
check "2.6 忽略规则命中 build" "build/app.bin" "$(git check-ignore build/app.bin || true)"
check "2.6 忽略规则命中日志" "logs/run.log" "$(git check-ignore logs/run.log || true)"
check "2.6 忽略规则命中权重" "weights/model.pt" "$(git check-ignore weights/model.pt || true)"
check "2.6 忽略规则命中本地配置" "config.local.yaml" "$(git check-ignore config.local.yaml || true)"
check "2.6 被忽略的路径有四个" "4" "$(git status --ignored --short | grep -c '^!!')"
git add .gitignore README.md src/main.py
gc -m "初始提交：程序与忽略规则" >/dev/null
git switch -q -c feature/score-format
cat > src/main.py <<'EOF'
scores = [12, 7, 9]
print(f"总分：{sum(scores)}")
EOF
git add src/main.py
gc -m "feat: 输出中加上总分说明" >/dev/null
git switch -q main
gmerge --no-edit --no-ff feature/score-format -m "合并：输出中加上总分说明" >/dev/null
git branch -d feature/score-format >/dev/null
check "2.6 有一次合并提交" "合并：输出中加上总分说明" "$(git log --merges --format=%s)"
check "2.6 只剩主分支" "main" "$(git branch --format='%(refname:short)')"
git add -f config.local.yaml
gc -m "chore: 添加本地配置" >/dev/null
git rm --cached config.local.yaml >/dev/null
gc -m "chore: 移除误提交的本地配置" >/dev/null
check "2.6 误提交的文件不再被跟踪" "0" "$(git ls-files | grep -c 'config.local.yaml')"
check "2.6 跟踪的文件" ".gitignore
README.md
src/main.py" "$(git ls-files)"
check "2.6 最近一次提交" "chore: 移除误提交的本地配置" "$(git log -1 --format=%s)"
check "2.6 工作区干净" "" "$(git status --porcelain)"
check "2.6 忽略规则还在生效" "config.local.yaml" "$(git check-ignore config.local.yaml || true)"
cd "$work/示例代码" || exit 1

# ── 3.1 文本与编码 ────────────────────────────────────
printf '\n3.1 文本与编码\n'
c31="$work/示例代码/3.1-文本与编码"
check "3.1 utf8.txt 的类型" "Unicode text, UTF-8 text" "$(file -b "$c31/utf8.txt")"
check "3.1 ascii.txt 的类型" "ASCII text" "$(file -b "$c31/ascii.txt")"
check "3.1 gbk.txt 被识别为非 UTF-8" "ISO-8859 text" "$(file -b "$c31/gbk.txt")"
check "3.1 bom.txt 的类型" "Unicode text, UTF-8 (with BOM) text" "$(file -b "$c31/bom.txt")"
check "3.1 crlf.txt 的类型" "Unicode text, UTF-8 text, with CRLF line terminators" "$(file -b "$c31/crlf.txt")"
check "3.1 utf8.txt 是 27 字节" "27" "$(wc -c < "$c31/utf8.txt")"
check "3.1 bom.txt 是 30 字节" "30" "$(wc -c < "$c31/bom.txt")"
check "3.1 crlf.txt 是 22 字节" "22" "$(wc -c < "$c31/crlf.txt")"
check "3.1 utf8.txt 的前六个字节" "e79bb8e69cba" "$(od -An -tx1 -N 6 "$c31/utf8.txt" | tr -d ' \n')"
check "3.1 bom.txt 以 EF BB BF 开头" "efbbbfe79bb8" "$(od -An -tx1 -N 6 "$c31/bom.txt" | tr -d ' \n')"
check "3.1 GBK 转 UTF-8 之后的内容" "相机
分辨率 1280x1024" "$(iconv -f GBK -t UTF-8 "$c31/gbk.txt")"

# ── 3.2 JSON ──────────────────────────────────────────
printf '\n3.2 JSON\n'
c32="$work/示例代码/3.2-JSON"
check "3.2 读取配置的五行输出" "相机: cam0
分辨率: 1280 x 1024
帧率: 30
曝光: 8000 us
触发: software" "$(cd "$c32" && python3 read_config.py)"
check "3.2 json.tool 能通过 config.json" "0" "$(python3 -m json.tool "$c32/config.json" >/dev/null 2>&1; echo $?)"
check "3.2 json.tool 输出 18 行" "18" "$(python3 -m json.tool "$c32/config.json" | wc -l)"
check "3.2 broken.json 的退出码是 1" "1" "$(python3 -m json.tool "$c32/broken.json" >/dev/null 2>&1; echo $?)"
check "3.2 broken.json 的报错位置" "Expecting property name enclosed in double quotes: line 4 column 1 (char 33)" "$(python3 -m json.tool "$c32/broken.json" 2>&1 >/dev/null | tail -n 1)"

# ── 3.3 YAML ──────────────────────────────────────────
printf '\n3.3 YAML\n'
c33="$work/示例代码/3.3-YAML"
check "3.3 camera 是一级键" "1" "$(grep -c '^camera:$' "$c33/camera.yaml")"
check "3.3 steps 有三项" "3" "$(grep -c '^  - ' "$c33/camera.yaml")"
check "3.3 notes 使用字面块" "1" "$(grep -c '^notes: |$' "$c33/camera.yaml")"
check "3.3 calibration 留空" "1" "$(grep -c '^calibration: null$' "$c33/camera.yaml")"
check "3.3 YAML 与 JSON 的对应值" "data/images
3
1024
None" "$(cd "$c33" && python3 -c 'import json; c = json.load(open("same.json", encoding="utf-8")); print(c["camera"]["save"]["directory"]); print(len(c["steps"])); print(c["camera"]["resolution"][1]); print(c["calibration"])')"

# ── 3.4 Markdown ──────────────────────────────────────
printf '\n3.4 Markdown\n'
c34="$work/示例代码/3.4-Markdown"
check "3.4 README 是 25 行" "25" "$(wc -l < "$c34/README.md")"
check "3.4 一共四个标题" "4" "$(grep -c '^#' "$c34/README.md")"
check "3.4 标题与行号" "1:# 计分工具
5:## 用法
11:## 输入格式
18:## 说明" "$(grep -n '^#' "$c34/README.md")"
check "3.4 表格有四行" "4" "$(grep -c '^|' "$c34/README.md")"
check "3.4 代码围栏成对" "2" "$(grep -c '^```' "$c34/README.md")"
check "3.4 链接目标存在" "0" "$(test -f "$c34/docs/usage.md"; echo $?)"

# ── 3.5 Shell 脚本 ────────────────────────────────────
printf '\n3.5 Shell 脚本\n'
c35="$work/示例代码/3.5-Shell脚本"
check "3.5 脚本的统计结果" "条数: 3
总分: 259
平均分: 86" "$(cd "$c35" && bash summary.sh)"
out=$(cd "$c35" && bash summary.sh missing.txt 2>&1); code=$?
check "3.5 缺文件时的提示" "找不到文件: missing.txt" "$out"
check "3.5 缺文件时的退出码" "1" "$code"
check "3.5 默认参数" "name: world
count: 1" "$(cd "$c35" && bash check_args.sh)"
check "3.5 传入两个参数" "name: alice
count: 3" "$(cd "$c35" && bash check_args.sh alice 3)"
check "3.5 脚本语法正确" "0" "$(bash -n "$c35/summary.sh"; echo $?)"
check "3.5 初始权限是 664" "664" "$(stat -c '%a' "$c35/summary.sh")"
chmod +x "$c35/summary.sh"
check "3.5 加执行权限后是 775" "775" "$(stat -c '%a' "$c35/summary.sh")"
chmod 664 "$c35/summary.sh"

# ── 3.6 模型文件 ──────────────────────────────────────
printf '\n3.6 模型文件\n'
c36="$work/示例代码/3.6-模型文件"
check "3.6 导出脚本调用 torch.onnx.export" "1" "$(grep -c 'torch.onnx.export' "$c36/export_example.py")"
check "3.6 opset 版本是 17" "1" "$(grep -c 'opset_version=17' "$c36/export_example.py")"
check "3.6 导出脚本语法正确" "0" "$(python3 -m py_compile "$c36/export_example.py"; echo $?)"

# ── 3.7 日志 ──────────────────────────────────────────
printf '\n3.7 日志\n'
c37="$work/示例代码/3.7-日志"
check "3.7 日志是 11 行" "11" "$(wc -l < "$c37/app.log")"
check "3.7 日志是 584 字节" "584" "$(wc -c < "$c37/app.log")"
check "3.7 前三行" "2026-03-14 09:00:01 INFO  camera: 打开设备 cam0
2026-03-14 09:00:01 INFO  camera: 分辨率 1280x1024, 帧率 30
2026-03-14 09:00:02 WARN  camera: 曝光时间 8000 us 偏大" "$(head -n 3 "$c37/app.log")"
check "3.7 后三行" "2026-03-14 09:02:00 ERROR save: 写入失败: 权限不足
2026-03-14 09:02:01 INFO  save: 重试成功
2026-03-14 09:03:00 INFO  camera: 关闭设备 cam0" "$(tail -n 3 "$c37/app.log")"
check "3.7 两行 ERROR" "2026-03-14 09:00:10 ERROR capture: 丢帧 3 帧
2026-03-14 09:02:00 ERROR save: 写入失败: 权限不足" "$(grep ERROR "$c37/app.log")"
check "3.7 ERROR 的行号" "5:2026-03-14 09:00:10 ERROR capture: 丢帧 3 帧
9:2026-03-14 09:02:00 ERROR save: 写入失败: 权限不足" "$(grep -n ERROR "$c37/app.log")"
check "3.7 WARN 有两行" "2" "$(grep -c WARN "$c37/app.log")"
check "3.7 各级别行数" "      2 ERROR
      7 INFO
      2 WARN" "$(awk '{print $3}' "$c37/app.log" | sort | uniq -c)"
check "3.7 第 4 到 6 行" "2026-03-14 09:00:03 INFO  capture: 开始采集
2026-03-14 09:00:10 ERROR capture: 丢帧 3 帧
2026-03-14 09:00:11 INFO  capture: 恢复采集" "$(sed -n '4,6p' "$c37/app.log")"
check "3.7 旧日志是 4 行" "4" "$(wc -l < "$c37/app.log.1")"

# ── 第 4 部分用 C++：编译命令与学生手敲的一致 ─────────
run_cpp() {
    ( cd "$1" && g++ -std=c++17 -Wall -Wextra -O2 "$2" -o demo && ./demo )
}

# ── 4.1 向量与矩阵 ────────────────────────────────────
printf '\n4.1 向量与矩阵\n'
c41="$work/示例代码/4.1-向量与矩阵"
check "4.1 例子的输出" "点积 = 11.000
a 的长度 = 2.236
A b = [11.000, 25.000]
A B = [[2.000, 7.000], [6.000, 15.000]]
B A = [[5.000, 8.000], [9.000, 12.000]]" "$(run_cpp "$c41" vectors.cpp)"

# ── 4.2 行列式 ────────────────────────────────────────
printf '\n4.2 行列式\n'
c42="$work/示例代码/4.2-行列式"
check "4.2 例子的输出" "2×2 行列式 = 5
3×3 行列式 = 8
三角形面积 = 6.000" "$(run_cpp "$c42" determinant.cpp)"

# ── 4.3 秩 ────────────────────────────────────────────
printf '\n4.3 秩\n'
c43="$work/示例代码/4.3-秩"
check "4.3 例子的输出" "行简化：
[1, 2, 0]
[0, 0, 1]
[0, 0, 0]
秩 = 2
增广矩阵的解：唯一解" "$(run_cpp "$c43" rank.cpp)"

# ── 4.4 转置与逆 ──────────────────────────────────────
printf '\n4.4 转置与逆\n'
c44="$work/示例代码/4.4-转置与逆"
check "4.4 例子的输出" "A2 的逆：
[[0.600, -0.200], [-0.200, 0.400]]
A3 的逆：
[[0.750, -0.500, 0.250], [-0.500, 1.000, -0.500], [0.250, -0.500, 0.750]]
RᵀR = [[1.000, 0.000], [0.000, 1.000]]" "$(run_cpp "$c44" inverse.cpp)"

# ── 4.5 坐标变换 ──────────────────────────────────────
printf '\n4.5 坐标变换\n'
c45="$work/示例代码/4.5-坐标变换"
check "4.5 例子的输出" "旋转 90° = [0.000, 1.000]
旋转后平移 = [2.000, 2.000]
复合矩阵作用于同一点 = [2.000, 2.000]
逆变换送回 = [1.000, 0.000]
先 T1 再 T2 = [-1.000, -2.000]" "$(run_cpp "$c45" transform.cpp)"

# ── 4.6 特征值 ────────────────────────────────────────
printf '\n4.6 特征值与特征向量\n'
c46="$work/示例代码/4.6-特征值"
check "4.6 例子的输出" "特征值 = 3.000, 1.000
较大特征值的单位特征向量 = [0.707, 0.707]
Q Λ Qᵀ = [[2.000, 1.000], [1.000, 2.000]]
幂迭代的主特征值 = 4.732" "$(run_cpp "$c46" eigen.cpp)"

# ── 4.7 最小二乘 ──────────────────────────────────────
printf '\n4.7 最小二乘\n'
c47="$work/示例代码/4.7-最小二乘"
check "4.7 例子的输出" "a = 1.700, b = 0.000
残差平方和 = 0.300
x = 5 时的预测值 = 8.500" "$(run_cpp "$c47" least_squares.cpp)"

# ── 4.8 奇异值分解 ────────────────────────────────────
printf '\n4.8 奇异值分解\n'
c48="$work/示例代码/4.8-奇异值分解"
check "4.8 例子的输出" "奇异值 = 6.325, 3.162
秩 1 近似 = [[2.000, 2.000], [4.000, 4.000]]
条件数 = 2.000" "$(run_cpp "$c48" svd.cpp)"

# ── 第 5 部分用 Python：算例不需要相机 ───────────────
run_py() {
    ( cd "$1" && python3 -B "$2" )
}

# 网络例子里每次运行都会变的端口与伪终端编号，先换成占位符再比。
norm_net() {
    sed -E 's/127\.0\.0\.1:[0-9]+/127.0.0.1:<port>/g; s#/dev/pts/[0-9]+#/dev/pts/<n>#g'
}
run_py_net() {
    run_py "$1" "$2" | norm_net
}

# ── 5.1 成像原理 ──────────────────────────────────────
printf '\n5.1 成像原理\n'
c51="$work/示例代码/5.1-成像原理"
check "5.1 例子的输出" "(0, 0, 2000) mm -> 像素 (640.000, 512.000)
(300, -200, 2000) mm -> 像素 (760.000, 432.000)
(0, 0, 4000) mm -> 像素 (640.000, 512.000)
水平视场角 = 77.320°
垂直视场角 = 65.238°" "$(run_py "$c51" projection.py)"

# ── 5.2 传感器 ────────────────────────────────────────
printf '\n5.2 传感器\n'
c52="$work/示例代码/5.2-传感器"
check "5.2 例子的输出" "相机 A：1280 x 1024
  像元 10.00 um，靶面 12.800 x 10.240 mm
  对角线 16.392 mm，8 bit 每帧 1.311 MB
相机 B：2448 x 2048
  像元 3.45 um，靶面 8.446 x 7.066 mm
  对角线 11.011 mm，8 bit 每帧 5.014 MB" "$(run_py "$c52" sensor.py)"

# ── 5.3 镜头 ──────────────────────────────────────────
printf '\n5.3 镜头\n'
c53="$work/示例代码/5.3-镜头"
check "5.3 例子的输出" "需要焦距 = 21.333 mm，选 16 mm
实际水平视场 = 400.000 mm（500 mm 处）
放大倍率 = 0.0320
每像素约 0.3125 mm
F4.0、弥散圆 0.0106 mm 时：
景深近界 = 462.984 mm，远界 = 543.449 mm
景深 = 80.465 mm" "$(run_py "$c53" lens.py)"

# ── 5.4 曝光 ──────────────────────────────────────────
printf '\n5.4 曝光\n'
c54="$work/示例代码/5.4-曝光"
check "5.4 例子的输出" "基准：F4.0, 10000 us, 0 dB
快门 5000 us 的等价设置：
  F2.8, 10000 us, 0 dB（光圈开一档）
  F4.0, 10000 us, 6 dB（增益加一档）
运动模糊：速度 0.500 m/s，曝光 5000 us -> 8.000 像素
50 Hz 光源：曝光 10000 us = 1.000 个波动周期
50 Hz 光源：曝光 7500 us = 0.750 个波动周期" "$(run_py "$c54" exposure.py)"

# ── 5.5 接口 ──────────────────────────────────────────
printf '\n5.5 接口\n'
c55="$work/示例代码/5.5-接口"
check "5.5 例子的输出" "1280 x 1024 Mono8（按 8 bit 打包）：1.311 MB/帧
1280 x 1024 Mono10（按 10 bit 打包）：1.638 MB/帧
1280 x 1024 Mono12（按 12 bit 打包）：1.966 MB/帧
GigE：Mono8 最多 95.367 fps（理论带宽，未扣协议开销）
USB 3.0：Mono8 最多 476.837 fps（理论带宽，未扣协议开销）
10GigE：Mono8 最多 953.674 fps（理论带宽，未扣协议开销）
Mono8、100 fps 需要 131.072 MB/s
GigE 只有 125.000 MB/s：带宽不足，会丢帧" "$(run_py "$c55" bandwidth.py)"

# ── 5.6 采集流程 ──────────────────────────────────────
printf '\n5.6 采集流程\n'
c56="$work/示例代码/5.6-采集流程"
check "5.6 例子的输出" "缓冲 5 帧，生产 10 ms，消费 12 ms，模拟 1000 ms：
  产生 100 帧，取走 83 帧，缓冲里剩 4 帧，丢弃 13 帧
缓冲 5 帧，生产 10 ms，消费 8 ms，模拟 1000 ms：
  产生 100 帧，取走 100 帧，缓冲里剩 0 帧，丢弃 0 帧" "$(run_py "$c56" capture.py)"

# ── 5.7 标定 ──────────────────────────────────────────
printf '\n5.7 标定\n'
c57="$work/示例代码/5.7-标定"
check "5.7 例子的输出" "内参：fx = 800.000, fy = 800.000, cx = 640.000, cy = 512.000
畸变：k1 = -0.120, k2 = 0.050
点 1：投影 (640.000, 512.000)，观测 (640.400, 511.700)，误差 0.500 像素
点 2：投影 (679.985, 492.007)，观测 (680.300, 491.800)，误差 0.377 像素
点 3：投影 (592.027, 537.586)，观测 (591.700, 537.900)，误差 0.453 像素
重投影误差 RMS = 0.446 像素" "$(run_py "$c57" calibration.py)"

# ── 第 6 部分：只连回环地址的网络例子 ─────────────────
# ── 6.1 IP 与端口 ─────────────────────────────────────
printf '\n6.1 IP 与端口\n'
c61="$work/示例代码/6.1-IP与端口"
check "6.1 例子的输出" "监听地址：127.0.0.1:<port>
客户端本地地址：127.0.0.1:<port>
客户端连接的对端地址：127.0.0.1:<port>
服务端收到：ping
客户端收到：pong" "$(run_py_net "$c61" ip_port.py)"

# ── 6.2 TCP 与 UDP ────────────────────────────────────
printf '\n6.2 TCP 与 UDP\n'
c62="$work/示例代码/6.2-TCP与UDP"
check "6.2 TCP 例子的输出" "客户端：一次 sendall 发送两行
服务端第 1 行：10,20
服务端第 2 行：30,40" "$(run_py_net "$c62" tcp_stream.py)"
check "6.2 UDP 例子的输出" "客户端：两次 sendto 发送两个数据报
服务端第 1 个数据报：10,20
服务端第 2 个数据报：30,40" "$(run_py_net "$c62" udp_datagram.py)"

# ── 6.3 HTTP 与 JSON ──────────────────────────────────
printf '\n6.3 HTTP 与 JSON\n'
c63="$work/示例代码/6.3-HTTP与JSON"
check "6.3 例子的输出" "GET /config
状态：200
Content-Type：application/json; charset=utf-8
camera = A，exposure_us = 5000，enabled = True
POST /echo
状态：200
发送：{\"a\": 3, \"b\": 4}
收到：{\"received\": {\"a\": 3, \"b\": 4}, \"sum\": 7}
GET /missing
状态：404
错误：{\"error\": \"not found\"}
POST /echo（Content-Type 不是 JSON）
状态：415
服务线程已退出：True
剩余线程数：1" "$(run_py_net "$c63" http_json.py)"

# ── 6.4 本地服务 ──────────────────────────────────────
printf '\n6.4 本地服务\n'
c64="$work/示例代码/6.4-本地服务"
check "6.4 自检的输出" "GET / 状态：200，类型：text/html; charset=utf-8
页面标题：本地服务 · 运行状态
GET /status 状态：200，类型：application/json; charset=utf-8
{\"hits\": 2, \"message\": \"服务运行正常\", \"ok\": true}
服务线程已退出：True" "$(run_py_net "$c64" local_service.py)"

# ── 6.5 串口到网页 ────────────────────────────────────
printf '\n6.5 串口到网页\n'
c65="$work/示例代码/6.5-串口到网页"
check "6.5 自检的输出" "模拟串口路径：/dev/pts/<n>
GET /latest 状态：200，类型：application/json; charset=utf-8
{\"bad_lines\": 0, \"count\": 3, \"latest\": {\"seq\": 3, \"temperature\": 25.6, \"voltage\": 12.2}}
GET / 状态：200，类型：text/html; charset=utf-8
页面标题：串口数据 · 本地网页
串口读取线程已退出：True
服务线程已退出：True" "$(run_py_net "$c65" serial_to_web.py)"

# ── 6.6 排错 ──────────────────────────────────────────
printf '\n6.6 排错\n'
c66="$work/示例代码/6.6-排错"
check "6.6 例子的输出" "1. 连接一个没有监听的端口
   异常：ConnectionRefusedError
   判断：本机没有进程监听这个端口，服务可能没有启动。
2. 同一个端口绑定两次
   异常：OSError
   errno：98（EADDRINUSE）
   判断：端口已被占用，换一个端口或先停止占用它的进程。
3. 连接成功但对方不回应
   异常：TimeoutError
   判断：连接已建立，但 0.2 秒内没有收到数据；检查对端是否发送、协议是否一致。
4. 请求一个不存在的 HTTP 路径
   状态码：404
   判断：服务在运行，路径写错了；404 是正常响应，不是连接失败。
   服务线程已退出：True
5. 解析一段不完整的 JSON
   异常：JSONDecodeError
   位置：第 1 行第 9 列
   判断：数据不完整或被截断；先检查发送端与分隔方式。" "$(run_py_net "$c66" diagnose.py)"

# ── 第 7 部分：建立工程与分支流程 ─────────────────────
# ── 7.1 从空目录到小工程 ──────────────────────────────
printf '\n7.1 从空目录到小工程\n'
c71="$work/示例代码/7.1-从空目录到小工程"
chmod +x "$c71/run.sh"
check "7.1 启动脚本的输出" "相机：camera A
分辨率：1280 × 1024
像元：10.0 µm
焦距：8.0 mm
焦距折算：800.0 px（fx=800.0，fy=800.0）
水平视场角：77.32°
垂直视场角：65.24°
单帧：1310720 字节（1.25 MiB）
30 fps 数据量：37.50 MB/s
已写入 report.json" "$(cd "$c71" && ./run.sh)"
check "7.1 report.json 的关键值" "77.319617 65.238486 1310720 37.5" "$(cd "$c71" && python3 -c 'import json; r = json.load(open("report.json", encoding="utf-8")); print(r["horizontal_fov_deg"], r["vertical_fov_deg"], r["frame_bytes"], r["bandwidth_mib_s"])')"
git -C "$c71" init -q -b master
check "7.1 report.json 被 .gitignore 忽略" "report.json" "$(git -C "$c71" check-ignore report.json || true)"

# ── 7.2 从功能分支到 V1.1 ─────────────────────────────
printf '\n7.2 从功能分支到 V1.1\n'
c72="$work/示例代码/7.2-从功能分支到V1.1"
out=$(cd "$c72" && bash walkthrough.sh 2>&1); code=$?
check "7.2 分支流程通过" "0
7.2 分支流程检查通过" "$code
$(printf '%s\n' "$out" | tail -n 1)"

printf '\n════════════════════════════════════════════\n'
printf ' 汇总：%d 项通过，%d 项不对\n' "$pass" "$fail"
printf '════════════════════════════════════════════\n'

rm -rf "$work"
[ "$fail" -eq 0 ]
