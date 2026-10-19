// 2.4 用：命令桌那个程序。三件事——
//   报一下版本（那个头文件是 configure_file 生成的，不在源码目录里）
//   调一下工具库里的函数（那个库是 add_library 建出来的）
//   看一眼那个开关开没开（#cmakedefine 生成的宏）

#include <iostream>

#include "version.h"
#include "util.h"

int main() {
    std::cout << "版本 " << VERSION_TEXT << " —— " << DESC_TEXT << std::endl;
    std::cout << "工具库说：双倍(21) = " << 双倍(21) << std::endl;
#ifdef EXTRA_LINE
    std::cout << "这一行是那个开关打开的。" << std::endl;
#endif
    return 0;
}
