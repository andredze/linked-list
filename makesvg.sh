#!/bin/bash

# Данный скрипт строит графы с помощью dot (graphviz) по файлам из graphs/dot/

for dir in log/*; do
    for file in ${dir}/dot/*.dot; do
        filename=$(basename "$file" .dot)
        dot "$file" -T svg -o "${dir}/svg/${filename}.svg"
    done
    # xdg-open ${dir}/list_log.html
done

echo -e "Converted dot log files to svg\n"

for dir in std_log/*; do
    for file in ${dir}/dot/*.dot; do
        filename=$(basename "$file" .dot)
        dot "$file" -T svg -o "${dir}/svg/${filename}.svg"
    done
done

echo -e "Converted dot std_log files to svg\n"
