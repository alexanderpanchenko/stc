# Building the console clinet of the short text classification library

####################
# Parameters

CC=g++ -m32 
FLAGS=-O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP
INCLUDE=./include
# The object files are saved in next to sources (client or library directories)
# The binaries (std and libshort_text_classifier.so) are saved in the bin directory.

####################
# The console client

all: stc

stc: ./client/main.o ./bin/libshort_text_classifier.so
	$(CC) -L./bin -o ./bin/stc ./client/main.o -lshort_text_classifier 
./client/main.o: 
	$(CC) -I./library -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./client/main.d" -MT"./client/main.d" -o ./client/main.o ./client/main.cpp 

#####################
# The shared library  

./bin/libshort_text_classifier.so: ./library/BowVector.o ./library/Classifier.o ./library/ClassifierManager.o ./library/FreqVocabulary.o ./library/TextExpander.o ./library/Vocabulary.o ./library/predict.o ./library/share_headers.o ./library/textclassifier.o ./library/train.o 
	$(CC) -L./bin -shared -o ./bin/libshort_text_classifier.so ./library/BowVector.o ./library/Classifier.o ./library/ClassifierManager.o ./library/FreqVocabulary.o ./library/TextExpander.o ./library/Vocabulary.o ./library/predict.o ./library/share_headers.o ./library/textclassifier.o ./library/train.o -lboost_regex -lboost_system -lboost_filesystem -llinear

./library/BowVector.o:  ./library/BowVector.cpp ./library/BowVector.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/BowVector.d" -MT"./library/BowVector.d" -o ./library/BowVector.o ./library/BowVector.cpp
 
./library/Classifier.o: ./library/Classifier.cpp ./library/Classifier.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/Classifier.d" -MT"./library/Classifier.d" -o ./library/Classifier.o ./library/Classifier.cpp

./library/ClassifierManager.o: ./library/ClassifierManager.cpp ./library/ClassifierManager.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/ClassifierManager.d" -MT"./library/ClassifierManager.d" -o ./library/ClassifierManager.o ./library/ClassifierManager.cpp
 
./library/FreqVocabulary.o: ./library/FreqVocabulary.cpp ./library/FreqVocabulary.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/FreqVocabulary.d" -MT"./library/FreqVocabulary.d" -o ./library/FreqVocabulary.o ./library/FreqVocabulary.cpp

./library/TextExpander.o: ./library/TextExpander.cpp ./library/TextExpander.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/TextExpander.d" -MT"./library/TextExpander.d" -o ./library/TextExpander.o ./library/TextExpander.cpp
 
./library/Vocabulary.o: ./library/Vocabulary.cpp ./library/Vocabulary.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/Vocabulary.d" -MT"./library/Vocabulary.d" -o ./library/Vocabulary.o ./library/Vocabulary.cpp

./library/predict.o: ./library/predict.cpp ./library/linear.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/predict.d" -MT"./library/predict.d" -o ./library/predict.o ./library/predict.cpp
 
./library/share_headers.o: ./library/share_headers.cpp ./library/share_headers.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/share_headers.d" -MT"./library/share_headers.d" -o ./library/share_headers.o ./library/share_headers.cpp
 
./library/textclassifier.o: ./library/textclassifier.cpp ./library/textclassifier.h ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/textclassifier.d" -MT"./library/textclassifier.d" -o ./library/textclassifier.o ./library/textclassifier.cpp
 
./library/train.o: ./library/train.cpp ./library/linear.hpp ./library/share_headers.hpp
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/train.d" -MT"./library/train.d" -o ./library/train.o ./library/train.cpp
 
clean:  
	rm -f ./client/*.o ./client/*.d ./library/*.o ./library/*.d ./bin/libshort_text_classifier.so ./bin/stc ./client/stc ./library/libshort_text_classifier.so
