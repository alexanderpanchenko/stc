#!/bin/bash

# Process the parameters
if [ $# -lt 1 ] ; then
	echo "This script trains a new classifier"
	echo "Usage: train.sh <model-dir>"  
	echo "<model-dir> A directory which contains a training collection (train.xml), a stoplist (stopwords.csv), a stopos list (stopos.csv) and a list of relations (relations.csv). The model (model, and vocabulary.csv) will be saved in this directory."
	exit
fi
model=$1

# Constants
icop=./../bin/stc
verbose="-v true" # to quite use "", not "-v false"

# Train 
$icop -M t -i $model/train.xml -m $model/model -V $model/vocabulary.csv -s $model/stopwords.csv -c $model/stopos.csv -N u -r $model/relations.csv 

# Validate
./classify.sh $model/valid.xml $model
