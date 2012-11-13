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

stc: main.o libshort_text_classifier.so
	$(CC) -L./bin -o ./bin/stc ./client/main.o -lshort_text_classifier 
main.o: 
	$(CC) -I./library -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./client/main.d" -MT"./client/main.d" -o ./client/main.o ./client/main.cpp 

#####################
# The shared library  

libshort_text_classifier.so: BowVector.o Classifier.o ClassifierManager.o FreqVocabulary.o TextExpander.o Vocabulary.o classifier_c.o predict.o share_headers.o textclassifier.o train.o 
	$(CC) -L./bin -shared -o ./bin/libshort_text_classifier.so ./library/BowVector.o ./library/Classifier.o ./library/ClassifierManager.o ./library/FreqVocabulary.o ./library/TextExpander.o ./library/Vocabulary.o ./library/classifier_c.o ./library/predict.o ./library/share_headers.o ./library/textclassifier.o ./library/train.o -lboost_regex -lboost_system -lboost_filesystem -llinear

BowVector.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/BowVector.d" -MT"./library/BowVector.d" -o ./library/BowVector.o ./library/BowVector.cpp
 
Classifier.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/Classifier.d" -MT"./library/Classifier.d" -o ./library/Classifier.o ./library/Classifier.cpp

ClassifierManager.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/ClassifierManager.d" -MT"./library/ClassifierManager.d" -o ./library/ClassifierManager.o ./library/ClassifierManager.cpp
 
FreqVocabulary.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/FreqVocabulary.d" -MT"./library/FreqVocabulary.d" -o ./library/FreqVocabulary.o ./library/FreqVocabulary.cpp

TextExpander.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/TextExpander.d" -MT"./library/TextExpander.d" -o ./library/TextExpander.o ./library/TextExpander.cpp
 
Vocabulary.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/Vocabulary.d" -MT"./library/Vocabulary.d" -o ./library/Vocabulary.o ./library/Vocabulary.cpp

classifier_c.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/classifier_c.d" -MT"./library/classifier_c.d" -o ./library/classifier_c.o ./library/classifier_c.cpp
 
predict.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/predict.d" -MT"./library/predict.d" -o ./library/predict.o ./library/predict.cpp
 
share_headers.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/share_headers.d" -MT"./library/share_headers.d" -o ./library/share_headers.o ./library/share_headers.cpp
 
textclassifier.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/textclassifier.d" -MT"./library/textclassifier.d" -o ./library/textclassifier.o ./library/textclassifier.cpp
 
train.o: 
	$(CC) -I$(INCLUDE) $(FLAGS) -MF"./library/train.d" -MT"./library/train.d" -o ./library/train.o ./library/train.cpp
 
clean:  
	rm -f ./client/*.o ./client/*.d ./library/*.o ./library/*.d ./bin/libshort_text_classifier.so ./bin/stc ./client/stc ./library/libshort_text_classifier.so
