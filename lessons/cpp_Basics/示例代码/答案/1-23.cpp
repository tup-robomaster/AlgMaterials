// 练习 1-23（找错）—— 想去掉小数位，结果小数位全回来了

#include <cstdio>

int main() {
    printf("%.2f\n", 19.9);
    printf("%2f\n", 19.9);

    return 0;
}
