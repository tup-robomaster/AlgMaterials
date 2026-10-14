#!/bin/bash
# 把示例代码里每个能跑的程序编译一遍、跑一遍，打印真实输出。
# 用途：拿这份输出和课里写的「预期输出」对照。两边必须逐字符一致。
#
# 用法：  bash verify.sh
#
# 名字决定它归哪一桶，扫的时候按名字分：
#
#   普通名字        编译应该干净，跑起来应该正常收场。
#   带 -err 的      故意写错的，只编译（必定失败），用来展示真实报错。
#                   这几个故意写得极简、连注释都没有，好让报错里的行号
#                   和课里展示的源码逐行对得上。
#   带 warn 的      故意写成「能编译但有警告」的样子（2.2 讲 = 和 &&
#                   那两个坑，2.1 那条缩进陷阱也算）。警告本身就是课里
#                   要贴的东西，所以这一桶不算失败——反过来，要是编译器
#                   不吭声了，才是真出错。
#   带 hang 的      故意死循环的（2.3）。只跑 2 秒，只留前 3 行。
#
# 1.3-divzero.cpp 是故意让它崩的（除以 0），跑起来异常退出，这是对的。
# 4.1-write-oob-crash.cpp 和 4.2-write-oob-crash.cpp 也是故意让它崩的
# （往数组外面写，压到栈哨兵），退出码 134。它们都该
# 「编译干净、跑起来非 0」，和它们旁边那份不崩的（4.1/4.2-write-oob.cpp）
# 配成一对——课里讲的就是这个对比。
#
# 要读输入的程序（1.5-*），输入从同名的 .in 文件里喂（1.5-many.in）。
# 没有 .in 的就接 /dev/null——不然它会停在键盘上等你，这个循环就卡死了。
# 喂了什么会打出来，这样你知道这份输出是在哪个输入下跑出来的。
#
# 每个程序都套了 timeout。不套的话，任何一个卡住的程序都会让
# 这个循环跟着卡住——不报错、不退出，就是不动，最难查。
#
# 答案/ 那个子文件夹这里不管（这个循环只扫当前目录）。想核自己写的答案，
# 就进那个文件夹，一份一份编译、跑，跟课里每个练习下面的「预期输出」对。

cd "$(dirname "$0")" || exit 1

pass=0
fail=0
deliberate=0

echo "══════════════════════════════════════════════════"
echo " 能跑的程序：编译 + 运行"
echo "══════════════════════════════════════════════════"

for f in *.cpp; do
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

    # 死循环那份单独走：输出倒进文件再取头几行。不这么办的话，
    # 2 秒是三百多万行，全塞进 shell 变量里。
    case "$f" in
    *hang*)
        timeout 2 /tmp/cppcourse-check > /tmp/cppcourse-out 2>&1
        rc=$?
        printf '  ── 实际输出（只留前 3 行）──\n'
        head -3 /tmp/cppcourse-out | sed 's/^/    /'
        printf '    ……后面还在一行一行地打，没完\n'
        printf '  ── 跑满 2 秒被 timeout 掐掉，退出码 %d（124 就是超时）──\n' "$rc"
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
        printf '  ⚠ 程序异常退出（退出码 %d）。这是故意的——1.3 除以 0、\n' "$rc"
        printf '     4.1 和 4.2 里那两份 -crash 的，都是这个样子。\n'
    fi
    printf '  ── 输出结束 ──\n'
done

echo
echo "══════════════════════════════════════════════════"
echo " 故意写错的程序：应该编译失败，报错信息如下"
echo "══════════════════════════════════════════════════"

for f in *-err*.cpp; do
    [ -e "$f" ] || continue
    printf '\n── %s ──\n' "$f"
    if errout=$(g++ -std=c++17 -Wall -o /tmp/cppcourse-check "$f" 2>&1); then
        printf '  ✗ 这一份本该编译不过，编译器却放它过去了\n'
        fail=$((fail + 1))
        [ -n "$errout" ] && printf '%s\n' "$errout" | sed 's/^/  /'
    else
        printf '%s\n' "$errout" | sed 's/^/  /'
    fi
done

echo
echo "══════════════════════════════════════════════════"
printf ' 汇总：%d 个正常编译运行，%d 个故意冒警告，%d 个有问题\n' "$pass" "$deliberate" "$fail"
echo "══════════════════════════════════════════════════"
