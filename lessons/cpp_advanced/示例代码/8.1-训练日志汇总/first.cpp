// 第一版：所有事情都写在 main 里，没有错误处理。只用于和最后的工程对照。
// 它不参与 CMakeLists.txt 的构建。

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Record {
    std::string date;
    int id;
    int score;
};

int main() {
    std::ifstream in("records.txt");
    std::vector<Record> records;
    Record r;
    while (in >> r.date >> r.id >> r.score) {
        records.push_back(r);
    }

    int total = 0;
    for (const Record& x : records) {
        total += x.score;
    }
    double average = total * 1.0 / records.size();

    const Record* best = &records[0];
    for (const Record& x : records) {
        if (x.score > best->score) {
            best = &x;
        }
    }

    std::map<int, int> by_id;
    for (const Record& x : records) {
        by_id[x.id] += x.score;
    }

    int tenths = static_cast<int>(average * 10.0 + 0.5);
    std::cout << "训练日志汇总\n";
    std::cout << "场次：" << records.size() << "\n";
    std::cout << "总分：" << total << "\n";
    std::cout << "平均：" << tenths / 10 << "." << tenths % 10 << "\n";
    std::cout << "最高：" << best->score << "（" << best->date
              << "，编号 " << best->id << "）\n";
    for (const auto& item : by_id) {
        std::cout << "编号 " << item.first << "：" << item.second << " 分\n";
    }

    std::ofstream out("report.txt");
    out << "训练报告\n";
    out << "场次：" << records.size() << "\n";
    out << "总分：" << total << "\n";
    out << "平均：" << tenths / 10 << "." << tenths % 10 << "\n";
    out << "最高：" << best->score << "\n";
    for (const auto& item : by_id) {
        out << "编号 " << item.first << "：" << item.second << " 分\n";
    }

    std::cout << "已写入 report.txt\n";
    return 0;
}
