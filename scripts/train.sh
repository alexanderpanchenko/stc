#!/bin/bash

# This script applies a model trained on galleries/wikipedia to a new file

# Process the command line arguments
if [ $# -lt 1 ] ; then
	echo "This script trains a new classifier"
	echo "Usage: train.sh <model-dir>"  
	echo "<model-dir> A directory which contains a training collection (train.xml), a stoplist (stopwords.csv), a stopos list (stopos.csv) and a list of relations (relations.csv). The model (model, and vocabulary.csv) will be saved in this directory."
	exit
fi
model=$1

# Set the paths to the data and the programs
icop=~/work/active/icop/src/icop-console/Release/stc

# Classify the copy of the file
$icop -M t -i $model/train.xml -m $model/model -V $model/vocabulary.csv -s $model/stopwords.csv -c $model/stopos.csv -N u -r $model/relations.csv 

# Apply the model on the validation set
./classify.sh $model/valid.xml $model
