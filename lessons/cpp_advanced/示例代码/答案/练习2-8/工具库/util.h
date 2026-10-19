// 2.4 用：工具库里的那个函数。头文件和 .cpp 都在 工具库/ 这个子目录里，
// 用它的 main.cpp 在上一层——所以子目录那份 CMakeLists.txt 里
// target_include_directories 写的是 PUBLIC：谁链这个库，谁就能找到这个头文件。

#ifndef TOOLS_H
#define TOOLS_H

int 双倍(int x);

#endif
