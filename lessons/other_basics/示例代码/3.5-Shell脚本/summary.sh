#!/bin/bash
set -eu

file=${1:-scores.txt}

if [ ! -f "$file" ]; then
    echo "找不到文件: $file" >&2
    exit 1
fi

count=0
sum=0
while IFS= read -r line; do
    if [ -z "$line" ]; then
        continue
    fi
    count=$((count + 1))
    sum=$((sum + line))
done < "$file"

echo "条数: $count"
echo "总分: $sum"
echo "平均分: $((sum / count))"
