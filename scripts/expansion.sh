#!/bin/bash

# This script pefroms various runs of text classification with text expansion of the unclassified text.
# Various strategies and parameters are tested: t - first-order, second-order, or recursive; n - number 
# relations per word; w - weight of each expanded term w.r.t. original terms. 

# Process the command line arguments
if [ $# -lt 2 ] ; then
	echo "Usage: expansion.sh <xml-texts> <relations>"  
	exit 0
fi

valid=$1
relations=$2
outdir=./../data/relations/ww/results-41729
stc=./../bin/latest/stc
modeldir=./../data/test
fname=$RANDOM.xml

cp $valid $outdir/$fname
echo ""
echo "=================BASE========================"
echo "input:$valid"
echo "output:$outdir/$fname" 
echo "relations: $relations"
$stc -M p -i $outdir/$fname -m $modeldir/model -V $modeldir/vocabulary.csv -s $modeldir/stopwords.csv -c $modeldir/stopos.csv -N u -r $relations  
	
# Try different parameters type X weights X number
for t in 1 2 
do
#t=1
	for w in 1 0.75 0.5 0.25 0.1 0.05 
	do
		for n in 1 3 5 10 20
		do
		#n=3
	
			# Create a new file for classification
			relname=${relations##*/}
			cp $valid $outdir/$fname

			# Classify the text with text expansion
			echo ""
			echo "========================================="
			echo "relations=$relname, type=$t num=$n, weight=$w" 
			$stc -M p -i $outdir/$fname -m $modeldir/model -V $modeldir/vocabulary.csv -s $modeldir/stopwords.csv -c $modeldir/stopos.csv -N u -r $relations -R $t -W $w -E $n 

		done	
	done
done
