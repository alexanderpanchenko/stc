#!/bin/bash
echo "3"
./preprocess-wiki-titles.py /home/sasha/Desktop/labels_en.nq /home/sasha/Desktop/labels-3.csv 3
echo "0"
./preprocess-wiki-titles.py /home/sasha/Desktop/labels_en.nq /home/sasha/Desktop/labels-0.csv 0
echo "1"
./preprocess-wiki-titles.py /home/sasha/Desktop/labels_en.nq /home/sasha/Desktop/labels-1.csv 1
echo "2"
./preprocess-wiki-titles.py /home/sasha/Desktop/labels_en.nq /home/sasha/Desktop/labels-2.csv 2
