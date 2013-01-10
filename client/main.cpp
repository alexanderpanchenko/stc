// Modes of the program
#define PREDICT "predict"
#define TRAIN "train"
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
#include <getopt.h>

#include "ClassifierManager.hpp"
#include "Classifier.hpp"
#include "TextExpander.hpp"
#include "share_headers.hpp"

using namespace std;

// Represents common global variables
struct GlobalArgs {
    // Name of the concepts file
    string inputFile;

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
void help_message(const char* fileName);
void print_message(const char* format, ...);
void run_test();

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
	else{
		printf("Error: unknown program mode.\n");
	}
	delete classifier;

	print_message("Finished.");
	return 0;
}

void version_message(){
	printf("stc %s\n",VERSION);
	printf("Copyright (C) Cental, UCL, 2012 Alexander Panchenko\n");
}

void print_message(const char* format, ...){
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

void help_message(const char * fileName){
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
 * Otherwise return false.
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

void run_test(){
	/*
	*/
}
