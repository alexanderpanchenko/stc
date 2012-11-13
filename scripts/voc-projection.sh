#!/bin/bash

#for k in 0 1 3 5 10 15 20 30 50 100
for k in 30 
do
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=gallery-train, test=gallery-valid"
	#./classify.sh ./../data/gallery/valid.xml ./../data/gallery/ $k 
	
	echo ""
	echo "============================================"
	echo "k=$k, train=gallery-train, test=piratebay-TT-train"
	./classify.sh ./../data/tt/train.xml ./../data/gallery/ $k 
	
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=gallery-train, test=piratebay-TT-valid"
	#./classify.sh ./../data/tt/valid.xml ./../data/gallery/ $k 
	
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=gallery-train, test=piratebay-TDT-train"
	#./classify.sh ./../data/tdt/train.xml ./../data/test/ $k 
	
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=gallery-train, test=piratebay-TDT-train"
	#./classify.sh ./../data/tdt/valid.xml ./../data/test/ $k
	
	
	#echo "" 
	#echo "============================================"
	#echo "k=$k, train=piratebay-TT-train, test=gallery-train,"
	#./classify.sh ./../data/test/train.xml ./../data/tt/ $k 
	
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=piratebay-TT-train, test=gallery-valid,"
	#./classify.sh ./../data/test/valid.xml ./../data/tt/ $k 

	#echo ""
	#echo "============================================"
	#echo "k=$k, train=piratebay-TDT-train, test=gallery-train,"
	#./classify.sh ./../data/test/train.xml ./../data/tdt/ $k 

	#echo ""
	#echo "============================================"
	#echo "k=$k, train=piratebay-TDT-train, test=gallery-valid,"
	#./classify.sh ./../data/test/valid.xml ./../data/tdt/ $k 
	
	#echo ""
	#echo "============================================"
	#echo "k=$k, train=piratebay-TT-train, test=piratebay-TT-valid,"
	#./classify.sh ./../data/tt/valid.xml ./../data/tt/ $k 

	#echo ""
	#echo "============================================"
	#echo "k=$k, train=piratebay-TDT-train, test=piratebay-TDT-valid,"
	#./classify.sh ./../data/tdt/valid.xml ./../data/tdt/ $k 
done

#for k in 0 1 3 5 10 15 20 30 50 100
#do
#	echo "============================================"
#	echo "k=$k, test=gallery-valid"
#	./classify.sh ~/Desktop/icop/test/valid.xml ~/Desktop/icop/test/ $k 

#	echo "============================================"
#	echo "k=$k, test=piratebay-TT-train"
#	./classify.sh ~/Desktop/icop/tt/train.xml ~/Desktop/icop/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TT-valid"
#	./classify.sh ~/Desktop/icop/tt/valid.xml ~/Desktop/icop/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TDT-train"
#	./classify.sh ~/Desktop/icop/tdt/train.xml ~/Desktop/icop/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TDT-train"
#	./classify.sh ~/Desktop/icop/tdt/valid.xml ~/Desktop/icop/test/ $k 
#done

#for k in 0 1 3 5 10 15 20 30 50 100
#do
#	echo "============================================"
#	echo "k=$k, test=gallery-valid"
#	./classify.sh ./../data/test/valid.xml ./../data/test/ $k 

#	echo "============================================"
#	echo "k=$k, test=piratebay-TT-train"
#	./classify.sh ./../data/tt/train.xml ./../data/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TT-valid"
#	./classify.sh ./../data/tt/valid.xml ./../data/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TDT-train"
#	./classify.sh ./../data/tdt/train.xml ./../data/test/ $k 
	
#	echo "============================================"
#	echo "k=$k, test=piratebay-TDT-train"
#	./classify.sh ./../data/tdt/valid.xml ./../data/test/ $k 
#done
