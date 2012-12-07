#!/bin/bash

# Removes temporary files

for dir in `ls -d -- ./../data/*/`
do
	rm -f $dir/train.xml-*
	rm -f $dir/valid.xml-*
	rm -f $dir/train.csv
	rm -f $dir/valid.csv
	rm -f $dir/model.csv
	rm -f $dir/README
done
