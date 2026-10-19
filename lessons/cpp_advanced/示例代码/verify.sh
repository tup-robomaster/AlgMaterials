#!/bin/bash
# 把示例代码里每个能跑的程序编译一遍、跑一遍，打印真实输出。
# 用途：拿这份输出和课里写的「预期输出」对照。两边必须逐字符一致。
#
# 用法：  bash verify.sh
#
# 这门课有两圈要扫，比基础课多一圈：
#
#   第一圈：当前目录下的 .cpp——单文件例子（0.2、1.1 那种），一个文件一个程序。
#           名字带 -err 的故意写错，只编译（必定失败）；名字带 warn / hang
#           的桶跟基础课一样（这门课目前还没有）。要读输入的从同名 .in 喂，
#           没有 .in 的接 /dev/null。
#
#   第二圈：子目录——一个子目录就是一个多文件工程（1.3-两个文件 这种）。
#           怎么编怎么跑不在这儿猜：照它自己那份 构建.txt，一行一条命令。
#           那份 构建.txt 同时是给学生看的（课里让他照着敲），所以它错不得：
#           这一圈就是拿真 g++ 把每一条真跑一遍。目录名带 -err 的（1.2-重复包含-err、
#           1.5-少链接-err）要求**必定失败**——课上贴的就是那一段报错。
#           工程整个拷到 /tmp 里跑，跑完删掉：.o、可执行文件、cmake 的 build/
#           一个都不许落回仓库。答案/ 没有 构建.txt，这一圈自动跳过它。
#
# 每个程序、每条命令都套了 timeout。不套的话，任何一个卡住的程序都会让
# 这个循环跟着卡住——不报错、不退出，就是不动，最难查。
#
# 答案/ 那个子文件夹这里不管：它没有 构建.txt，这一圈自动跳过。
# 想核答案，把 答案/ 里的 .cpp 编译跑一遍，和网页上每个练习下面的
#「预期输出」逐行对；网页上贴的程序和这些 .cpp 是同一份。

cd "$(dirname "$0")" || exit 1

pass=0
fail=0
deliberate=0
dirs_ok=0
dirs_err=0

echo "══════════════════════════════════════════════════"
echo " 第一圈 · 单文件程序：编译 + 运行"
echo "══════════════════════════════════════════════════"

for f in *.cpp; do
    [ -e "$f" ] || continue
    case "$f" in *-err*) continue ;; esac

    name="${f%.cpp}"
    printf '\n── %s ──\n' "$f"

    if ! g++ -std=c++17 -Wall -o /tmp/cppcourse-check "$f" 2>/tmp/cppcourse-err; then
        printf '  ✗ 编译失败\n'
        sed 's/^/    /' /tmp/cppcourse-err
        fail=$((fail + 1))
        continue
    fi

    case "$f" in
    *warn*)
        if [ -s /tmp/cppcourse-err ]; then
            printf '  ✓ 编译通过，编译器警告了——这是故意的，课里贴的就是这段：\n'
            sed 's/^/    /' /tmp/cppcourse-err
            deliberate=$((deliberate + 1))
        else
            printf '  ✗ 本该冒警告，编译器却一声没吭：课里贴的那段警告对不上了\n'
            fail=$((fail + 1))
        fi
        ;;
    *)
        if [ -s /tmp/cppcourse-err ]; then
            printf '  ⚠ 编译通过，但有警告：\n'
            sed 's/^/    /' /tmp/cppcourse-err
            fail=$((fail + 1))
        else
            printf '  ✓ 编译通过，无警告\n'
            pass=$((pass + 1))
        fi
        ;;
    esac

    case "$f" in
    *hang*)
        timeout 2 /tmp/cppcourse-check > /tmp/cppcourse-out 2>&1
        rc=$?
        printf '  ── 实际输出（只留前 3 行）──\n'
        head -3 /tmp/cppcourse-out | sed 's/^/    /'
        printf '    ……后面还在一行一行地打，没完\n'
        printf '  ── 跑满 2 秒被 timeout 中止，退出码 %d（124 就是超时）──\n' "$rc"
        continue
        ;;
    esac

    if [ -f "$name.in" ]; then
        printf '  ── 喂进去的输入（%s.in）──\n' "$name"
        sed 's/^/    /' "$name.in"
        out=$( { timeout 2 /tmp/cppcourse-check < "$name.in"; } 2>&1 )
    else
        out=$( { timeout 2 /tmp/cppcourse-check < /dev/null; } 2>&1 )
    fi
    rc=$?
    printf '  ── 实际输出 ──\n'
    [ -n "$out" ] && printf '%s\n' "$out" | sed 's/^/    /'
    if [ "$rc" -ne 0 ]; then
        printf '  ⚠ 程序异常退出（退出码 %d）。\n' "$rc"
    fi
    printf '  ── 输出结束 ──\n'
done

echo
echo "══════════════════════════════════════════════════"
echo " 第二圈 · 多文件工程：照每个目录的 构建.txt 真跑"
echo "══════════════════════════════════════════════════"

for d in */; do
    name="${d%/}"
    [ -f "$d/构建.txt" ] || continue

    printf '\n── %s/ ──\n' "$name"
    printf '  ── 构建.txt ──\n'
    sed 's/^/    /' "$d/构建.txt"
    for inf in "$d"*.in; do
        [ -e "$inf" ] || continue
        printf '  ── 喂进去的输入（%s）──\n' "$(basename "$inf")"
        sed 's/^/    /' "$inf"
    done

    # 工程整个拷进 /tmp 里跑：.o、可执行文件、build/ 都落在那边，仓库干干净净
    work=$(mktemp -d /tmp/cppcourse-dir-XXXXXX)
    cp -r "$d". "$work"/

    # 一条一条跑，但「现在在哪个目录」要带着走：0.3-cmake目录 那份就是
    # cd build 之后 cmake ..。cd 因此不进子 shell，单独认出来改写 cwd，
    # 其余每条从 cwd 出发、套 timeout（卡住的话整圈都会停在那儿不动）。
    cwd="$work"
    broke=0
    while IFS= read -r cmd; do
        [ -n "$cmd" ] || continue
        printf '  $ %s\n' "$cmd"
        case "$cmd" in
        cd\ *)
            if cwd=$(cd "$cwd" && cd "${cmd#cd }" && pwd); then
                printf '      （这一步之后在 %s）\n' "${cwd#$work/}"
            else
                printf '      （切不过去）\n'
                broke=$((broke + 1))
            fi
            continue
            ;;
        esac
        out=$( { cd "$cwd" && timeout 60 bash -c "$cmd"; } 2>&1 )
        rc=$?
        [ -n "$out" ] && printf '%s\n' "$out" | sed 's/^/      /'
        printf '      （退出码 %d）\n' "$rc"
        [ "$rc" -ne 0 ] && broke=$((broke + 1))
    done < "$d/构建.txt"

    rm -rf "$work"

    case "$name" in
    *-err*)
        if [ "$broke" -gt 0 ]; then
            printf '  ✓ 如上，这一份故意编不过——课里贴的就是这段报错\n'
            dirs_err=$((dirs_err + 1))
        else
            printf '  ✗ 这一份本该出错的，居然一路都过了：课里那段报错对不上了\n'
            fail=$((fail + 1))
        fi
        ;;
    *)
        if [ "$broke" -eq 0 ]; then
            printf '  ✓ 每一条都过了\n'
            dirs_ok=$((dirs_ok + 1))
        else
            printf '  ✗ 有 %d 条命令没过\n' "$broke"
            fail=$((fail + 1))
        fi
        ;;
    esac
done

echo
echo "══════════════════════════════════════════════════"
printf ' 汇总：%d 个单文件程序跑通，%d 个多文件工程跑通，' "$pass" "$dirs_ok"
printf '%d 个工程照剧本失败，\n' "$dirs_err"
printf '       %d 个故意冒警告，%d 个有问题\n' "$deliberate" "$fail"
echo "══════════════════════════════════════════════════"

[ "$fail" -eq 0 ]
