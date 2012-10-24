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

time $LIBSVM/svm-train -s 2 -t 0 $DIR/filenames.xml.train.csv $DIR/model20
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model20 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 2 -t 1 $DIR/filenames.xml.train.csv $DIR/model21
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model21 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 2 -t 2 $DIR/filenames.xml.train.csv $DIR/model22
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model22 $DIR/filenames.xml.valid.predict

time $LIBSVM/svm-train -s 2 -t 3 $DIR/filenames.xml.train.csv $DIR/model23
time $LIBSVM/svm-predict $DIR/filenames.xml.valid.csv $DIR/model23 $DIR/filenames.xml.valid.predict

