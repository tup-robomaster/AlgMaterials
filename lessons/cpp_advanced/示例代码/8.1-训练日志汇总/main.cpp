#include "report.h"

#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string path = "records.txt";
    if (argc > 1) {
        path = argv[1];
    }

    try {
        std::vector<Record> records = read_records(path);

        int total = 0;
        for (const Record& r : records) {
            total += r.score;
        }
        double average = total * 1.0 / records.size();
        const Record& best = best_record(records);
        std::map<int, int> by_id = group_totals(records);

        int tenths = static_cast<int>(average * 10.0 + 0.5);
        std::cout << "训练日志汇总\n";
        std::cout << "场次：" << records.size() << "\n";
        std::cout << "总分：" << total << "\n";
        std::cout << "平均：" << tenths / 10 << "." << tenths % 10 << "\n";
        std::cout << "最高：" << best.score << "（" << best.date
                  << "，编号 " << best.id << "）\n";
        for (const auto& item : by_id) {
            std::cout << "编号 " << item.first << "：" << item.second << " 分\n";
        }

        write_report("report.txt", records, total, average, best, by_id);
        std::cout << "已写入 report.txt\n";
    } catch (const std::exception& e) {
        std::cout << "处理失败：" << e.what() << "\n";
        return 1;
    }
    return 0;
}
