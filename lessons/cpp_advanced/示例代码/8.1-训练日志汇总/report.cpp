#include "report.h"

#include <stdexcept>

ReportWriter::ReportWriter(const std::string& path) : out_(path) {
    if (!out_) {
        throw std::runtime_error("打不开报告文件");
    }
}

ReportWriter::~ReportWriter() {
    out_.close();
}

std::ofstream& ReportWriter::out() {
    return out_;
}

std::vector<Record> read_records(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("打不开记录文件");
    }

    std::vector<Record> records;
    while (true) {
        Record r;
        if (!(in >> r.date)) {
            break;
        }
        if (!(in >> r.id >> r.score)) {
            throw std::runtime_error("记录文件格式错误");
        }
        records.push_back(r);
    }
    if (!in.eof()) {
        throw std::runtime_error("记录文件格式错误");
    }
    if (records.empty()) {
        throw std::runtime_error("记录文件是空的");
    }
    return records;
}

const Record& best_record(const std::vector<Record>& records) {
    if (records.empty()) {
        throw std::runtime_error("没有记录");
    }
    const Record* best = &records[0];
    for (const Record& r : records) {
        if (r.score > best->score) {
            best = &r;
        }
    }
    return *best;
}

std::map<int, int> group_totals(const std::vector<Record>& records) {
    std::map<int, int> totals;
    for (const Record& r : records) {
        totals[r.id] += r.score;
    }
    return totals;
}

void write_report(const std::string& path, const std::vector<Record>& records,
                  int total, double average, const Record& best,
                  const std::map<int, int>& by_id) {
    ReportWriter writer(path);
    std::ofstream& out = writer.out();

    int tenths = static_cast<int>(average * 10.0 + 0.5);
    out << "训练报告\n";
    out << "场次：" << records.size() << "\n";
    out << "总分：" << total << "\n";
    out << "平均：" << tenths / 10 << "." << tenths % 10 << "\n";
    out << "最高：" << best.score << "\n";
    for (const auto& item : by_id) {
        out << "编号 " << item.first << "：" << item.second << " 分\n";
    }
}
