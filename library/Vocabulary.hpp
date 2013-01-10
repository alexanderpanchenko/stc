#ifndef VOCABULARY_H_
#define VOCABULARY_H_

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include "share_headers.hpp"

using namespace std;

/*
 * Class stores list of terms loaded from a CSV file "term\n"
 * such as stopword list or stopos list and the like.
 * string=word, long=id
 */
class Vocabulary : public map<string, long>  {
public:
	Vocabulary(string file, bool verbose=false);
	bool contains(string word);
	long get_word_id(string word);
	virtual ~Vocabulary();
private:
	bool VERBOSE;
	bool load(string voc_file);
};

void test_simple_vocabulary(void);

#endif /* VOCABULARY_H_ */
