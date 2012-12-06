//   Modes of the program
#define PREDICT "predict"
#define TRAIN "train"
#define EXTRACT "extract"
#define VERSION "0.1"
#define CLIENT
#define TEST false
#define MEMORY_LEAK_TEST false

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "Classifier.hpp"
#include "TextExpander.hpp"
#include "share_headers.hpp"
#include "textclassifier.h"
#include <getopt.h>
#include "ClassifierManager.hpp"

using namespace std;

// Represents common global variables
struct GlobalArgs {
    // Name of the concepts file
    string inputFile;

    // Name of the output file
    //string featuresFile;

	// Name of the stop-words file
    string stopwordsFile;

    // Name of the file with stop parts of speech
    string stoposFile;

    // Vocabulary of a model
    string vocabularyFile;

    // Classification model
    string modelFile;

    // File with semantic relations
    string relationsFile;

    // Normalization of the input vector
    bool isUnitLength;

    // Verbose output
    bool verbose;

    // Mode of the program
    string mode;

    // Expansion parameters
    TextExpanderParams expansion_params;
} params;

bool load_params(int, char**);
void help_message(char * fileName);
void print_message(char* format, ...);
void run_test();
IniParam* get_iniparam();

int main(int argc, char* argv[]) {
	if (!load_params(argc, argv)) {
		help_message(argv[0]);
		return -1;
	}

	if(TEST) {
		run_test();
		return 0;
	}

	print_message("Started...");

	Classifier* classifier = new Classifier(params.stopwordsFile, params.stoposFile,
			params.vocabularyFile, params.modelFile, params.relationsFile, params.verbose);
	if(params.mode == PREDICT){
		if(MEMORY_LEAK_TEST) while(true) classifier->predict(params.inputFile, &(params.expansion_params), params.isUnitLength);
		else classifier->predict(params.inputFile, &(params.expansion_params), params.isUnitLength);
	}
	else if(params.mode == TRAIN){
		if(MEMORY_LEAK_TEST) while(true) classifier->train(params.inputFile, params.isUnitLength);
		else classifier->train(params.inputFile, params.isUnitLength);
	}
	else if(params.mode == EXTRACT){
		if(MEMORY_LEAK_TEST) while(true) classifier->predict(params.inputFile, params.inputFile + ".xml", &(params.expansion_params), params.isUnitLength);

		/*
		IniParam* ip = get_iniparam();
		void* cm = TextClassifier_Create(ip);
		TextClassifier_PrintInfo(cm);
		TextClassifier_Reload(cm, ip);
		TextClassifier_PrintInfo(cm);

		TextToClassify* text = classifier->load_texts(params.inputFile);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 1, text);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 2, text);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 3, text);

		TextClassifier_Train(cm, 3, text);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 1, text);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 2, text);
		TextClassifier_Predict(cm, (params.inputFile + ".xml").c_str(), 3, text);

		TextClassifier_PrintInfo(cm);

		TextClassifier_Destroy(cm);
		*/

		/*
		ClassifierManager* cm = new ClassifierManager(ip_begin);
		printf("%d\n", cm->get_models_num());
		cm->reload(ip_begin);
		printf("%d\n", cm->get_models_num());
		cm->get_model_ids();
		ClassifierManager* cm = get_classifier_manager();
		TextToClassify* text = classifier->load_texts(params.inputFile);
		cm->predict(1, text, params.inputFile + ".xml", &(params.expansion_params), params.isUnitLength);
		cm->predict(2, text, params.inputFile + ".xml", &(params.expansion_params), params.isUnitLength);
		cm->predict(1, text, params.inputFile + ".xml", &(params.expansion_params), params.isUnitLength);
		cm->predict(2, text, params.inputFile + ".xml", &(params.expansion_params), params.isUnitLength);
		cm->print_models_info();

		TextToClassify* traintext = classifier->load_texts("/home/sasha/Desktop/test2/filenames.xml.train");
		cm->train(3, traintext, params.isUnitLength);
		cm->predict(3, text, "/home/sasha/Desktop/test3/valid.xml", &(params.expansion_params), params.isUnitLength);
		cm->print_models_info();
		*/
	}
	delete classifier;

	print_message("Finished.");
	return 0;
}

void version_message(){
	printf("stc %s\n",VERSION);
	printf("Copyright (C) Cental, UCL, 2012 Alexander Panchenko\n");
}

void print_message(char* format, ...){
	char buffer[256];

	time_t rawtime;
	struct tm * timeinfo;
	char timeStr[30];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(timeStr, 30, "%d.%m.%Y %H:%M:%S",timeinfo);

	va_list args;
	va_start(args, format);
	vsprintf(buffer,format, args);
	va_end(args);

	fprintf(stderr, "[%s] %s\n", timeStr, buffer);
}

void help_message(char * fileName){
	char E=27;
	printf("%c[1m",E);
	printf("STC: a Short Text Classifier -- a command line tool for text classification.\n");
	printf("%c[0m",E);
	printf("This tool can train a text classification model and apply it on a new text or a text collection.\n");
	printf("\nUsage: %s [OPTIONS]\n", fileName);

	printf("%c[1m",E);printf("\n -M ");printf("%c[0m",E);
	printf("- mode, required:\n");
	printf("\t t - train a text classification model.\n");
	printf("\t p - classify text with an existing model.\n");
	printf("\t e - extract features from text.\n");

	printf("%c[1m",E);printf("\n -i ");printf("%c[0m",E);
	printf("- input XML file with texts, required.\n");

	printf("%c[1m",E);printf("\n -s");printf("%c[0m",E);
	printf(" - stopwords file, required. \n");

	printf("%c[1m",E);printf("\n -p");printf("%c[0m",E);
	printf(" - stop part of speech file, required. \n");

	printf("%c[1m",E);printf("\n -r");printf("%c[0m",E);
	printf(" - semantic relations file, required. \n");

	printf("%c[1m",E);printf("\n -V");printf("%c[0m",E);
	printf(" - vocabulary file, required. Input for prediction mode, output for the training mode.\n");

	printf("%c[1m",E);printf("\n -m");printf("%c[0m",E);
	printf(" - model file, required. Input for prediction mode, output for training mode.\n");

	printf("%c[1m",E);printf("\n -N");printf("%c[0m",E);
	printf(" - normalization of bag-of-word feature vectors, default 'u'. \n");
	printf("\t 0 - frequency counts of words (no normalization)\n");
	printf("\t 1 - unit length normalization\n");

	printf("%c[1m",E);printf("\n -v true");printf("%c[0m",E);
	printf(" - verbose output\n");

	printf("%c[1m",E);printf("\n -R");printf("%c[0m",E);
	printf(" - type of the text expansion, default '0'. \n");
	printf("\t 0 - no expansion\n");
	printf("\t 1 - first order expansion\n");
	printf("\t 2 - second order expansion\n");
	printf("\t 3 - recursive expansion\n");

	printf("%c[1m",E);printf("\n -E");printf("%c[0m",E);
	printf(" - number of per term expansions, default '0'. \n");

	printf("%c[1m",E);printf("\n -W");printf("%c[0m",E);
	printf(" - weight [0;1] of the expanded terms w.r.t. original terms, default '0'. \n");

	printf("%c[1m",E);printf("\n -h");printf("%c[0m",E);
	printf(" - print this help message\n");

	printf("\nAuthor:\n");
	printf("\tAlexander Panchenko, alexander.panchenko@uclouvain.be\n");
	printf("\tCopyright (C) Center for Natural Language Processing, Catholic University of Louvain, 2012\n");
}

static const char *optString = "c:i:s:h:?:V:v:N:m:M:t:p:e:r:R:W:E:";

/**
 * Parse the command line arguments and checks if they are correct.
 * Returns true if all required arguments were provided and are correct.
 * Otherwise returns false.
 * */
bool load_params(int argc, char* argv[]) {
	// Set default parameters (required)
	params.inputFile = "";
	params.stoposFile = "";
	params.stopwordsFile = "";
	params.vocabularyFile = "";
	params.modelFile = "";
	params.mode = "";

	// Set default parameters (not required)
	params.modelFile = "";
	params.isUnitLength = true;
	params.verbose = false;

	// Parse the arguments
	int opt = 0;
	opt = getopt( argc, argv, optString );
	while( opt != -1 ){
		switch( opt ){
		case 'i':// input file
			params.inputFile = string(optarg);
			break;
		case 's':// stopwords file
			params.stopwordsFile = string(optarg);
			break;
		case 'r':// semantic relations
			params.relationsFile = string(optarg);
			break;
		case 'c':// stopos file (syntactic categories)
			params.stoposFile = string(optarg);
			break;
		case 'm':// model file
			params.modelFile = string(optarg);
			break;
		case 'v': // verbose output
			params.verbose = true;
			break;
		case 'V': // vocabulary file
			params.vocabularyFile = string(optarg);
			break;
		case 'N': // Normalization of vectors
			switch(*optarg)	{
			case '0': // raw counts
				params.isUnitLength = false;
				break;
			case '1': // unit length
				params.isUnitLength = true;
				break;
			}
			break;
		case 'R': // Type of text expansion
			switch(*optarg){
			case '0':
				params.expansion_params.type = NO_EXP;
				break;
			case '1':
				params.expansion_params.type = FIRST_ORDER_EXP;
				break;
			case '2':
				params.expansion_params.type = SECOND_ORDER_EXP;
				break;
			case '3':
				params.expansion_params.type = RECURSIVE_EXP;
				break;
			}
			break;
		case 'E': // Max number of per term expansions
			params.expansion_params.max_expansions = atoi(optarg);
			break;
		case 'W': // Weigh of the expanded terms
			params.expansion_params.expansion_weight = atof(optarg);
			break;
		case 'M': // Mode
			switch(*optarg)	{
			case 't': // train
				params.mode = TRAIN;
				break;
			case 'p': // predict
				params.mode = PREDICT;
				break;
			case 'e': // extract features
				params.mode = EXTRACT;
				break;
			}
		break;

		case 'h':
		case '?':
			help_message(argv[0]);
			return false;
		}
		opt = getopt( argc, argv, optString );
	}

	// Check if all required arguments are set
	bool required_not_set =
			(params.inputFile == "") ||
			(params.stoposFile == "") ||
			(params.stopwordsFile == "") ||
			(params.vocabularyFile == "") ||
			(params.relationsFile == "") ||
			(params.modelFile == "") ||
			(params.mode == "");
	bool files_exist =
			file_exists(params.inputFile, true) &&
			file_exists(params.stopwordsFile, true) &&
			file_exists(params.relationsFile, true) &&
			file_exists(params.stoposFile, true);

	return !required_not_set && files_exist;
}


/*
void test1(){
	FreqVocabulary* fv = new FreqVocabulary("/home/sasha/Desktop/relations/vocabulary.csv");
		TextExpander* te = new TextExpander("/home/sasha/Desktop/relations/ww/concepts-41729.csv_dir/relations.csv", fv);

		
		for(long id=100; id < 200; id++){
			list<long> res = te->expand_id2id(id);
			if(res.size()>0){
				printf("%d: ", id);
				te->print_id_list(res);
			}
		}

		for(long id=100; id < 200; id++){
			list<string> res = te->expand_term2term(fv->get_word_by_id(id));
			if(res.size()>0){
				printf("%s: ", fv->get_word_by_id(id).c_str());
				te->print_term_list(res);
			}
		}
	

		int exp_num = 3;
		double w = 0.5;
		BowVector* br = new BowVector();
		BowVector* bf = new BowVector();
		BowVector* bs = new BowVector();
		int num=8; string title[8] = {"sex", "french", "student", "get", "fuck", "by", "her", "teacher"};
		//int num=6; string title[6] = {"big-tit", "milf", "shay", "fox", "get", "banged"};
		//int num = 2; string title[2] = {"sex", "french"};
		for(int i=0; i < num; i++) {
			long id = fv->get_wordid_ifexists(title[i]);
			if(id != -1){
				br->increment_word(id);
				bf->increment_word(id);
				bs->increment_word(id);
			}
		}
		printf("\noriginal:\n");
		te->print_vector(br);

		printf("\nrecursive:\n");
		br = te->enrich_vector_recursive(br, exp_num, w);
		te->print_vector(br);

		printf("\nfirst-order:\n");
		bf = te->enrich_vector(bf, exp_num, w, false);
		te->print_vector(bf);

		printf("\nsecond-order:\n");
		bs = te->enrich_vector(bs, exp_num, w, true);
		te->print_vector(bs);
}


void test2(){
	printf("test\n");
	//TextExpanderParams* p = new TextExpanderParams();
	TextExpanderParams p;
	if(p.type == SECOND_ORDER_EXP) printf("%d,%d,%f", (int)p.type, p.max_expansions, p.expansion_weight);
}


void run_test(){
	void* c = TextClassifier_Create("/home/sasha/Desktop/text_classifier_data/", 1);
	TextClassifier_PrintInfo(c);
	Classifier* classifier = new Classifier(params.stopwordsFile, params.stoposFile,
			params.vocabularyFile, params.modelFile, params.relationsFile, params.verbose);
	TextToClassify* traintext = classifier->load_texts("/home/sasha/Desktop/text_classifier_data/filenames.xml.valid");
	TextClassifier_Add(c, traintext);
	TextClassifier_PrintInfo(c);
	TextClassifier_Add(c, traintext);
	TextClassifier_Add(c, traintext);
	TextClassifier_Add(c, traintext);
	TextClassifier_PrintInfo(c);
	TextClassifier_Destroy(c);
}

IniParam* get_iniparam(){

IniParam* ip = new IniParam();
IniParam* ip_begin = ip;

char* s11 = new char[20];
sprintf(s11, "%s", "STOPWORDS_1");
ip->sKey = s11;
char* v11 = new char[100];
sprintf(v11, "%s", "/home/sasha/Desktop/test2/stopwords.csv");
ip->sVal = v11;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s21 = new char[20];
sprintf(s21, "%s", "STOPWORDS_2");
ip->sKey = s21;
char* v21 = new char[100];
sprintf(v21, "%s", "/home/sasha/Desktop/test2/stopwords.csv");
ip->sVal = v21;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s31 = new char[20];
sprintf(s31, "%s", "STOPWORDS_3");
ip->sKey = s31;
char* v31 = new char[100];
sprintf(v31, "%s", "/home/sasha/Desktop/test2/stopwords.csv");
ip->sVal = v31;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s12 = new char[20];
sprintf(s12, "%s", "STOPOS_1");
ip->sKey = s12;
char* v12 = new char[100];
sprintf(v12, "%s", "/home/sasha/Desktop/test2/stopos.csv");
ip->sVal = v12;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s22 = new char[20];
sprintf(s22, "%s", "STOPOS_2");
ip->sKey = s22;
char* v22 = new char[100];
sprintf(v22, "%s", "/home/sasha/Desktop/test2/stopos.csv");
ip->sVal = v22;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s32 = new char[20];
sprintf(s32, "%s", "STOPOS_3");
ip->sKey = s32;
char* v32 = new char[100];
sprintf(v32, "%s", "/home/sasha/Desktop/test2/stopos.csv");
ip->sVal = v32;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s13 = new char[20];
sprintf(s13, "%s", "VOCABULARY_1");
ip->sKey = s13;
char* v13 = new char[100];
sprintf(v13, "%s", "/home/sasha/Desktop/test/vocabulary.csv");
ip->sVal = v13;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s23 = new char[20];
sprintf(s23, "%s", "VOCABULARY_2");
ip->sKey = s23;
char* v23 = new char[100];
sprintf(v23, "%s", "/home/sasha/Desktop/test2/vocabulary.csv");
ip->sVal = v23;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s33 = new char[20];
sprintf(s33, "%s", "VOCABULARY_3");
ip->sKey = s33;
char* v33 = new char[100];
sprintf(v33, "%s", "/home/sasha/Desktop/test3/vocabulary.csv");
ip->sVal = v33;
ip->pNext = new IniParam();
ip = ip->pNext;

char* s14 = new char[20];
sprintf(s14, "%s", "MODEL_1");
ip->sKey = s14;
ip->sVal = "/home/sasha/Desktop/test/model";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s24 = new char[20];
sprintf(s24, "%s", "MODEL_2");
ip->sKey = s24;
ip->sVal = "/home/sasha/Desktop/test2/model";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s34 = new char[20];
sprintf(s34, "%s", "MODEL_3");
ip->sKey = s34;
ip->sVal = "/home/sasha/Desktop/test3/model";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s16 = new char[20];
sprintf(s16, "%s", "VERBOSE_1");
ip->sKey = s16;
ip->sVal = "false";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s26 = new char[20];
sprintf(s26, "%s", "VERBOSE_2");
ip->sKey = s26;
ip->sVal = "false";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s36 = new char[20];
sprintf(s36, "%s", "VERBOSE_3");
ip->sKey = s36;
ip->sVal = "false";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s15 = new char[20];
sprintf(s15, "%s", "RELATIONS_1");
ip->sKey = s15;
ip->sVal = "/home/sasha/Desktop/test2/relations.csv";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s35 = new char[20];
sprintf(s35, "%s", "RELATIONS_3");
ip->sKey = s35;
ip->sVal = "/home/sasha/Desktop/test2/relations.csv";
ip->pNext = new IniParam();
ip = ip->pNext;

char* s25 = new char[20];
sprintf(s25, "%s", "RELATIONS_2");
ip->sKey = s25;
ip->sVal = "/home/sasha/Desktop/test2/relations.csv";
ip->pNext = NULL;

return ip_begin;
}

*/
