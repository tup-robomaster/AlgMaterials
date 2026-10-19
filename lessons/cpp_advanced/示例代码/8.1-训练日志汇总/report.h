#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

struct Record {
    std::string date;
    int id;
    int score;
};

// 管理报告文件：构造时打开，析构时关闭；不允许拷贝。
// 两个对象同时写一份文件，写出来的行会交错，所以拷贝必须被禁止。
class ReportWriter {
public:
    explicit ReportWriter(const std::string& path);
    ~ReportWriter();

    ReportWriter(const ReportWriter&) = delete;
    ReportWriter& operator=(const ReportWriter&) = delete;

    std::ofstream& out();

private:
    std::ofstream out_;
};

std::vector<Record> read_records(const std::string& path);
const Record& best_record(const std::vector<Record>& records);
std::map<int, int> group_totals(const std::vector<Record>& records);

void write_report(const std::string& path, const std::vector<Record>& records,
                  int total, double average, const Record& best,
                  const std::map<int, int>& by_id);
