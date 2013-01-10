#ifndef FREQ_VOCABULARY_H_
#define FREQ_VOCABULARY_H_

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
 * Class provides a mapping "word-id" and contains frequency of words
 * string=word, long=id, long=frequency
 * The class can load/save the vocabulary index in the following format:
 * "word;id;frequency"
 * Basically, the class contains a bag-of-words model of a document collection.
 */
class FreqVocabulary : public map<string, pair<long, long> > {
public:
	FreqVocabulary(bool verbose=false);
	FreqVocabulary(string vocFile, bool verbose=false);
	bool load(string vocFile);
	bool save(string vocFile);
	long get_word_id(string word);
	bool exists(string word);
	long get_word_id_ifexists(string word);
	string get_word_by_id_debug(long id);
	pair<long, long> get_word_info(string word);
	bool increment_word(string word);
	void print_contents(void);
	virtual ~FreqVocabulary();
	pair<long, long> add_word(string word, bool negative_id=false);
private:
	bool VERBOSE;
};

// Test of the class
void vocabulary_test(void);

#endif /* VOCABULARY_H_ */

