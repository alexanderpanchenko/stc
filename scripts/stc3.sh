#!/bin/bash

# This script runs all the steps of the icop filename processing system:
# 1. Extraction of the training data from the input files; Sampling and construction of training and validation datasets
# 2. Transforming the datasets into the libsvm format
# 3. Training a LIB* classifier (10-folds CV) on the training dataset
# 4. Application of the model on the validation data

WIKI_TITLES=/home/sasha/Desktop/icop/labels-wiki/labels_en.nq #1M # Input file
DFKI_TITLES=/home/sasha/work/icop/data/dfki-all/all.xml # Input file
STOPLIST=/home/sasha/work/icop/data/test/stopwords.csv # Input resource
STOPOS=/home/sasha/work/icop/data/test/stopos.csv # Input resource

# Process the command line arguments
if [ $# -lt 1 ] ; then
	echo "Usage: icop.sh <output-dir>"  
	exit 0
fi
DIR=$1

mkdir $DIR

# 1. Extraction of the training data from the input files
./preprocess-wiki-titles.py $WIKI_TITLES $DIR/wiki-filenames.xml 500000 3
./preprocess-dfki-titles.py $DFKI_TITLES $DIR/dfki-filenames.xml
echo "<texts>" > $DIR/open.tag
echo "</texts>" > $DIR/close.tag
cat $DIR/open.tag $DIR/wiki-filenames.xml.train $DIR/dfki-filenames.xml.train $DIR/close.tag > $DIR/filenames.xml.train 
cat $DIR/open.tag $DIR/wiki-filenames.xml.valid $DIR/dfki-filenames.xml.valid $DIR/close.tag > $DIR/filenames.xml.valid
cp $DIR/filenames.xml.valid $DIR/filenames.xml.valid.original

# 2.,3. Training a default model on the training data
/home/sasha/work/icop/src/icop-console/Debug/icop-console -M t -i $DIR/filenames.xml.train  -m $DIR/model -V $DIR/vocabulary.csv -s $STOPLIST -c $STOPOS -N u

# 2. Transforming the datasets into the libsvm format
#/home/sasha/work/icop/src/cpp3/Release/icop-cpp-3 -M e -i $DIR/filenames.xml.train -V $DIR/vocabulary.csv -s $STOPLIST -c $STOPOS -N u

# 3. Training a LIBLINEAR classifier on the training dataset
#/home/sasha/work/icop/libs/liblinear-1.8/train $DIR/filenames.xml.train.csv $DIR/model

# 4. Application of the model on the validation data
/home/sasha/work/icop/src/icop-console/Debug/icop-console -M p -i $DIR/filenames.xml.valid -s $STOPLIST -c $STOPOS -N u -V $DIR/vocabulary.csv -m $DIR/model

# Delete the temporary files
#rm -f $DIR/wiki-filenames.xml*
#rm -f $DIR/dfki-filenames.xml*
#rm -f $DIR/*.tag
#rm -f $DIR/filenames.xml.valid.csv
#rm -f $DIR/filenames.xml.train.csv
