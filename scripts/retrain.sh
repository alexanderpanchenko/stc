#!/bin/bash

# Retrain all models in the ./../data/ directory
for dir in `ls -d -- ./../data/*/`
do
	echo; echo "========================================"
	echo "Training $dir"
	./train.sh $dir
done

# Cross-test the models 
for dir1 in `ls -d -- ./../data/*/`
do
	for dir2 in `ls -d -- ./../data/*/`
	do
		echo; echo "========================================"
		echo "Testing $dir1--$dir2"
		./classify.sh $dir1/train.xml $dir2 0
	done
done
