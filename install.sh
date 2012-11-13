#!/bin/bash
# Performs installations of the dynamic library

if [ -f /usr/lib32/libshort_text_classifier.so ]
then
	sudo mv /usr/lib32/libshort_text_classifier.so /usr/lib32/libshort_text_classifier.so.previous 
fi

sudo cp ./bin/libshort_text_classifier.so /usr/lib32/
sudo ldconfig
echo "The library libshort_text_classifier.so was installed in /usr/lib32/"
