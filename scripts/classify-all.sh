#!/bin/bash

# Process the command line arguments
if [ $# -lt 2 ] ; then
	echo "This script classifies all CSV files in a directory"
	echo "Usage: classify-dir.sh <input-dir> <model-dir>"  
	echo "<input-dir> A directory with CSV files. Each CSV file (one text per line) will be transformed to XML and then classified."
  	echo "<model-dir> A directry with a classification model"
	exit 0
fi

# Set the parameters
dir=$1
model=$2
files=$dir/*.csv
category=false

# Classify the files
for file in $files
do
	# Get the file name
	filename=$(basename "$file")
	filename="${filename%.*}"
	echo $filename

	# Classify the file
	./lemmatizer.py $dir/$filename.csv $dir/$filename.xml true $category true
	./classify.sh $dir/$filename.xml $model
done

