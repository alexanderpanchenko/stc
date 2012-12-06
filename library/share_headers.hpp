#ifndef SHARE_HEADERS_H_
#define SHARE_HEADERS_H_

// XML & LIBSVM filename formats

#define DEFAULT_STOPWORDS "stopwords.csv"
#define DEFAULT_STOPOS "stopos.csv"
#define DEFAULT_RELATIONS "relations.csv"
#define DEFAULT_VOCABULARY "vocabulary.csv"
#define DEFAULT_MODEL "model"

#define ROOT_TAG "texts"
#define TEXT_TAG "text"
#define ORIGINAL_TAG "original"
#define LEMMAS_TAG "lemmas"
#define CLASS_ATT "class"
#define CONFIDENCE_ATT "confidence"
#define DEFAULT_CONFIDENCE_ATT "1.0"
#define DEFAULT_UNKNOWN_CONFIDENCE_ATT "0.0"
#define TOKEN_SEPARATOR "#"
#define ID_ATT "id"

#define POSITIVE_CLASS_ATT "positive"
#define POSITIVE_CLASS "+1"
#define POSITIVE_CLASS_I +1

#define NEGATIVE_CLASS_ATT "negative"
#define NEGATIVE_CLASS "-1"
#define NEGATIVE_CLASS_I -1

// Other constants

#define DELETE_TMP false
#define MIN_FEATURES_NUM 1 // Minimum number of features in a bag-of-words vector
#define CLASS_NUM 2

#include<string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
// For file_exist functions
bool file_exists(char* fileName, bool silent);
bool file_exists(std::string fileName, bool silent);
std::string get_label_name(int label);

#endif /* SHARE_HEADERS_H_ */
