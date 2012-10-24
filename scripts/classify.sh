#!/bin/bash

# Process the command line arguments
if [ $# -lt 2 ] ; then
	echo "This script classifies a set of texts with a pre-trained model."
	echo "Usage: classify.sh <input-file> <model-dir>"  
	echo "<input-file> An XML file in the TCS format to clasify (not changed)"
	echo "<model-dir> A directry with a classification model"
	exit 0
fi

# Set the parameters
file=$1
model=$2
stc=/home/sasha/work/active/icop/src/icop-console/Release/stc

if [ $# -lt 3 ] ; then
	expansion_max=0
else
	expansion_max=$3
fi

# Make a copy of the file
copy=$1-$RANDOM
cp $file $copy 

# Classify the copy of the file
$stc -M p -i $copy -m $model/model -V $model/vocabulary.csv -s $model/stopwords.csv -c $model/stopos.csv -N u -r $model/relations.csv -E $expansion_max 
