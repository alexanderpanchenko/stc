#ifndef FEATUREEXTRACTOR_H_
#define FEATUREEXTRACTOR_H_

#ifndef CLIENT
#include <list>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#endif

#include "linear.hpp"
#include "Vocabulary.hpp"
#include "FreqVocabulary.hpp"
#include "BowVector.hpp"
#include "TextExpander.hpp"
#include "share_headers.hpp"
#include "textclassifier.h"

using namespace std;
#ifndef CLIENT
using namespace rapidxml;
using namespace boost;
using namespace boost::filesystem;
#endif

/**
 * This class implements the main functions of the text classification system.
 * It serves as an interface for console client stc.
 * Classifier which takes as input text from an XML file and classify it.
 * The class provides methods for training the models and prediction of new texts.
 * */
class Classifier {
public:
	Classifier(string stopwords_file, string stopos_file, string vocabulary_file,
			string model_file, string relations_file, bool verbose = false);
	virtual ~Classifier();
	bool train(string in_training_file,	bool is_unit_length=true);
	bool train(TextToClassify* text, bool is_unit_length=true);
	bool predict(TextToClassify* text, string output_file, TextExpanderParams* exp_params,
			bool is_unit_length=true);
	bool predict(string input_output_file, TextExpanderParams* exp_params, bool is_unit_length=true);
	bool predict(string input_file, string output_file, TextExpanderParams* exp_params, bool is_unit_length=true);
	list<bool> extract(string input_file, string output_file, TextExpanderParams* exp_params,
			bool is_unit_length=true);
	TextToClassify* load_texts(string input_file);
	void delete_text(TextToClassify* text);
	bool model_is_loaded();
	string get_stopwords_file();
	string get_stopos_file();
	string get_vocabulary_file();
	string get_model_file();
	string get_relations_file();
private:
	bool _verbose;
	bool _model_is_loaded;
	string _stopwords_file;
	string _stopos_file;
	string _vocabulary_file;
	string _model_file;
	string _relations_file;
	model* _model;
	Vocabulary* _stopwords;
	Vocabulary* _stopos;
	FreqVocabulary* _vocabulary;
	TextExpander* _expander;
	bool skip_token(string token);
	list<bool> text2vector(TextToClassify* texts, string output_file,
		TextExpanderParams* exp_params,	bool is_unit_length=true);
	void reconstruct_vector(string input_vector, string output_texts, string vocabulary_file);
	bool text2xml(TextToClassify* texts, list<bool> has_data, list<pred> labels, string output_file);

	TextToClassify* create_text();
	TokenToClassify* create_token();
	TokenToClassify* create_token(char* surface, char* pos, char* lemma);
	void delete_token(TokenToClassify* token);

	string token2string(TokenToClassify* token);
	bool load_train_resources();
	bool load_predict_resources();
	void unload_predict_resources();
};

#endif /* FEATUREEXTRACTOR_H_ */
