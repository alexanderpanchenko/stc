#ifndef TEXTEXPANDER_H_
#define TEXTEXPANDER_H_

#include <map>
#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "share_headers.hpp"
#include "FreqVocabulary.hpp"
#include "BowVector.hpp"

using namespace std;

/**
 * Performs various kinds of feature expansion of a text represented as a
 * bag-of-words  with help of a list of provided beforehand semantic relations
 * for the given bag-of-words.
 * The class itself is a map of terms (long) and related terms (list<long>).
 * Each position in the list corresponds to the rank.
 * */
class TextExpander : public map<long, list<long> >{
public:
	TextExpander(string relations_file, FreqVocabulary classifier_vocabulary, bool verbose = false);
	virtual ~TextExpander();
	list<string> get_related_terms_debug(string term);
	list<long> get_related_terms(long term_id);
	BowVector* enrich_vector_recursive(BowVector* text_vector, int max_expansions, double enrichment_weight);
	BowVector* enrich_vector(BowVector* text_vector, int max_expansions, double expansion_weight, bool second_expansion);
	list<long> get_projected_terms(string term, int max_related_terms);
	void print_id_list(list<long> id_list);
	void print_term_list(list<string> term_list);
	void print_vector(BowVector* text_vector);
	const FreqVocabulary& get_vocabulary() const { return _relations_vocabulary; }
private:
	bool VERBOSE;
	FreqVocabulary _relations_vocabulary;
	BowVector* get_enriched_vector(BowVector* original_vector, int max_expansions);
	void print_relations();
	void load_relations(string relations_file);
};

/**
 * These data structures store parameters of an expansion.
 * They have the same meaning as parameters of expand_* methods.
 * */
enum ExpansionType {
	NO_EXP = 0,
	FIRST_ORDER_EXP = 1,
	SECOND_ORDER_EXP = 2,
	RECURSIVE_EXP = 3
};

struct TextExpanderParams{
public:
	ExpansionType type;
	int max_expansions;
	double expansion_weight;
	TextExpanderParams() : type(NO_EXP), max_expansions(0), expansion_weight(0.0) { }
};

#endif /* TEXTEXPANDER_H_ */
