#!/bin/bash

WIKI_TITLES=/home/sasha/Desktop/icop/labels-wiki/labels_en.nq # Input file
DFKI_TITLES=/home/sasha/work/icop/data/dfki-all/all.xml # Input file
STOPLIST=/home/sasha/work/icop/data/test/stopwords.csv # Input resource
STOPOS=/home/sasha/work/icop/data/test/stopos.csv # Input resource

DIR=/home/sasha/Desktop/icop/lrec-results-scaled
LIBLINEAR=/home/sasha/work/icop/libs/liblinear-1.8
LIBSVM=/home/sasha/work/icop/libs/libsvm-3.11

#time $LIBLINEAR/train -s 7 $DIR/filenames.xml.train.csv $DIR/model
#time $LIBLINEAR/predict $DIR/filenames.xml.valid.csv $DIR/model $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 0 -t 0 $DIR/filenames.xml.train.csv $DIR/model00
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model00 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 0 -t 1 $DIR/filenames.xml.train.csv $DIR/model01
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model01 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 0 -t 2 $DIR/filenames.xml.train.csv $DIR/model02
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model02 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 0 -t 3 $DIR/filenames.xml.train.csv $DIR/model03
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model03 $DIR/filenames.xml.valid.predict

