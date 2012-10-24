#!/bin/bash

if [ -f /usr/lib32/libsimple_text_classifier.so ]
then
	sudo mv /usr/lib32/libsimple_text_classifier.so /usr/lib32/libsimple_text_classifier.so.previous 
fi

sudo cp libsimple_text_classifier.so /usr/lib32/
sudo ldconfig
