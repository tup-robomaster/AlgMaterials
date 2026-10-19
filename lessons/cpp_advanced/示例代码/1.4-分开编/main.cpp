// 1.4 用：拆完之后的 main——只剩三步：读、算、打。
//
// 注意这上面没有 #include <vector>，可下面用了 std::vector。
// 能编过，是因为 stats.h 里那行 #include <vector> 跟着贴进来了。
// （工程里的规矩是「谁用谁 include」，这儿是故意的，1.3 正文会拿它说事。）

#include "stats.h"

int main() {
    std::vector<int> scores = readScores(10);

    printReport(scores);
    return 0;
}
