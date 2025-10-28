#!/bin/bash

# Данный скрипт строит графы с помощью dot (graphviz) по файлам из graphs/dot/

for file in graphs/dot/*.dot; do

    filename=$(basename "$file" .dot)

    dot "$file" -T svg -o "graphs/svg/${filename}.svg"

done

echo -e "Converted graphs/dot/*.dot files to graphs/svg/*.svg\n"
