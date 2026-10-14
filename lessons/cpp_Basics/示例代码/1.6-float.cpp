// 1.6 补充：小数 %f 和 %.2f
//
// %f 不用你说，默认就留六位小数。要想只留两位，得在中间点一个点。

#include <cstdio>

int main() {
    double pi = 3.14159265;
    double price = 7.5;

    printf("%f\n", pi);
    printf("%.2f\n", pi);
    printf("%.3f\n", pi);

    printf("%f\n", price);
    printf("%.2f\n", price);

    printf("|%8.2f|\n", price);

    return 0;
}
