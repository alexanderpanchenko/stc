#!/bin/bash

model=$1

for i in {1..100}
do
	echo $i
	./create_dataset.py $model/positive.csv $model/negative.csv $model/valid.xml $model/train.xml
	./train.sh $model
done
