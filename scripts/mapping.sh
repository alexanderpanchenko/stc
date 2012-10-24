#!/bin/bash

for k in 0 1 3 5 10 15 20 30 50 100
do
	echo "============================================"
	echo "k=$k, test=gallery-valid"
	./classify.sh ~/Desktop/icop/test/valid.xml ~/Desktop/icop/test/ $k 

	echo "============================================"
	echo "k=$k, test=piratebay-TT-train"
	./classify.sh ~/Desktop/icop/tt/train.xml ~/Desktop/icop/test/ $k 
	
	echo "============================================"
	echo "k=$k, test=piratebay-TT-valid"
	./classify.sh ~/Desktop/icop/tt/valid.xml ~/Desktop/icop/test/ $k 
	
	echo "============================================"
	echo "k=$k, test=piratebay-TDT-train"
	./classify.sh ~/Desktop/icop/tdt/train.xml ~/Desktop/icop/test/ $k 
	
	echo "============================================"
	echo "k=$k, test=piratebay-TDT-train"
	./classify.sh ~/Desktop/icop/tdt/valid.xml ~/Desktop/icop/test/ $k 
done
