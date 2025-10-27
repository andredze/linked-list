#!/bin/bash

# Данный скрипт строит графы с помощью dot (graphviz) по файлам из graphs/txt/

for file in graphs/txt/*.txt; do

    filename=$(basename "$file" .txt)

    dot "$file" -T png -o "graphs/png/${filename}.png"

done
