#!/bin/bash
# This script cuts first three columns from the piratebay CSV files 
# The script modify the files, so it should be applied to a copy of the files
dir=/home/sasha/Desktop/icop/piratebay
find $dir -iname "*.csv" -exec sed -i "s/\([^;]*\);\([^;]*\);\([^;]*\);.\+/\1 ; \2 ; \3/g" '{}' \; -exec echo '{}' \;
