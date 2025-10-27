#!/bin/bash

# Данный скрипт строит графы с помощью dot (graphviz) по файлам из graphs/dot/

for file in graphs/dot/*.dot; do

    filename=$(basename "$file" .dot)

    dot "$file" -T png -o "graphs/png/${filename}.png"

done
