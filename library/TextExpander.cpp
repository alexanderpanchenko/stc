#include "TextExpander.hpp"

/**
* Constructor which recieve an initialized vocabulary, and a corresponding
* file with semantic relations relations_file. Each line of the latter file
* represents a relation: "term;related-term".
*/
TextExpander::TextExpander(string relations_file, FreqVocabulary classifier_vocabulary, bool verbose) {
VERBOSE = verbose;

	// Ensure that vocabulary is correctly initialized
	if(classifier_vocabulary.size() > 0){
		_relations_vocabulary = classifier_vocabulary;
	}
	else{
		_relations_vocabulary = new FreqVocabulary();
		printf("Error: vocabulary was not loaded.\n");
		return;
	}

	// Load semantic relations from the file
	load_relations(relations_file);
}

TextExpander::~TextExpander() { }

void TextExpander::print_relations() {
	map<long, list<long> >::iterator it;
	for (it = begin(); it != end(); it++){
		printf("%ld:%s =", it->first, _relations_vocabulary.get_word_by_id_debug(it->first).c_str());
		list<long>::iterator lit;

		for (lit = it->second.begin(); lit != it->second.end(); lit++){
			printf(" %ld:%s", *lit, _relations_vocabulary.get_word_by_id_debug(*lit).c_str());
		}
		printf("\n\n");
	}
}

/**
 * Perform an expansion for a given term with max_expansion terms
 * */
list<long> TextExpander::get_related_terms(long term_id) {
	map<long, list<long> >::iterator it = find(term_id);
	if(it != end())
		return (*it).second;
	list<long> void_list;
	return void_list;
}


/**
 * Perform an expansion for a given term with max_expansion terms.
 * This is very inefficient function -- for debug purposes only!
 * */
list<string> TextExpander::get_related_terms_debug(string term) {
	list<string> related;
	long term_id = _relations_vocabulary.get_word_id(term);
	if(term_id != 0){
		list<long> related_ids = get_related_terms(term_id);
		for(list<long>::iterator it = related_ids.begin(); it != related_ids.end(); it++){
			string related_term = _relations_vocabulary.get_word_by_id_debug(*it);
			related.push_back(related_term);
		}
	}
	return related;
}

/**
 * Recursive expansion. Perform an expansion for a provided bag-of-word vector on the term level.
 * Each term will be expanded with max_expansions terms, each with expansion_weight
 * from 0 to 1. The weights are additive.
 * */
BowVector* TextExpander::enrich_vector_recursive(BowVector* text_vector, int max_expansions, double expansion_weight) {
	// Check if input parameters are valid
	if(text_vector == NULL || expansion_weight > 1){
		printf("Warning: wrong parameters were provided to the expander\n");
		return text_vector;
	}

	// Increase weights of the initial features w.r.t. expanded features
	long mult_factor = (long)floor(1/expansion_weight);
	text_vector->multiply(mult_factor);

	// For each term in vector
	list<long> relatums;
	for(map<long,long>::iterator it_vector = text_vector->begin(); it_vector != text_vector->end(); it_vector++) {
		// Find the list of the related terms
		relatums = get_related_terms(it_vector->first);
		printf("\n>>>%s", _relations_vocabulary.get_word_by_id_debug(it_vector->first).c_str());

		// Increase the related terms for not more than max_expansions
		int expansions_num;
		list<long>::iterator it_relatums;
		for(it_relatums = relatums.begin(), expansions_num = 0; it_relatums != relatums.end(); it_relatums++, expansions_num++)	{
			if(expansions_num >= max_expansions) break;
			text_vector->increment_word(*it_relatums);
		}
																		}
		//printf("\n");

		// Return the expanded bag-of-words vector
		return text_vector;
}

/**
 * Return a vector with a term-level expansion for a provided vector.
 * */
BowVector* TextExpander::get_enriched_vector(BowVector* original_vector, int max_expansions) {
	if(VERBOSE) printf("\n");
	// For each term in vector
	BowVector* expansion_vector = new BowVector();
	list<long> relatums;
	for (map<long, long>::iterator it_vector = original_vector->begin(); it_vector != original_vector->end(); it_vector++) {
		// Find the list of the related terms
		relatums = get_related_terms(it_vector->first);
		if(VERBOSE) printf("  >>>%s", _relations_vocabulary.get_word_by_id_debug(it_vector->first).c_str());
		// Increase the related terms for not more than max_expansions
		int expansions_num;
		list<long>::iterator it_relatums;
		for (it_relatums = relatums.begin(), expansions_num = 0;
			it_relatums != relatums.end();	it_relatums++, expansions_num++) {
			if (expansions_num >= max_expansions || *it_relatums < 0)
				break;
			expansion_vector->increment_word(*it_relatums);
			if(VERBOSE) printf(">%s", _relations_vocabulary.get_word_by_id_debug(*it_relatums).c_str());
		}
	}
	if(VERBOSE) printf("#%d", expansion_vector->size());
		return expansion_vector;
}

/**
 * Perform an expansion for a provided bag-of-word vector on the term level.
 * Each term will be expanded with max_expansions terms, each with expansion_weight
 * from 0 to 1. The weights are additive.
 * If second expansion is true then expansion of expansion is done.
 * In that case v = a^2*original + a*first + second, a = 1/expansion_weight.
 * Otherwise v = a*original + first.
 * */
BowVector* TextExpander::enrich_vector(BowVector* original_vector, int max_expansions, double expansion_weight, bool second_expansion) {
	// Check if input parameters are valid
	if(original_vector == NULL || expansion_weight > 1){
		printf("Warning: wrong parameters were provided to the expander\n");
		return original_vector;
	}

	// Perform expansion
	BowVector* first_vector = get_enriched_vector(original_vector, max_expansions);
		long mult_factor = (long)(floor(1/expansion_weight));
		if(!second_expansion){
			// First order expansion
			original_vector->multiply(mult_factor);
			original_vector->add(first_vector);
		}
		else{
			// Second order expansion
			BowVector* second_vector = get_enriched_vector(first_vector, max_expansions);
			original_vector->multiply(mult_factor*mult_factor);
			first_vector->multiply(mult_factor);
			original_vector->add(first_vector);
			original_vector->add(second_vector);
		}
		return original_vector;
}

/**
 * Loading relations from a CSV file "term;related-term;\n".
 * The relatum terms of each target should be sorted by relevance.
 * */
void TextExpander::load_relations(string relations_file) {
	if (VERBOSE) printf("Loading relations file: %s...\n", relations_file.c_str());

	// Open the file
	ifstream relations_stm;
	relations_stm.open(relations_file.c_str());
	if (!relations_stm){
		printf("Error: can not open file with relations %s", relations_file.c_str());
		return;
	}

	// Read line by line
	string line, term_target, term_relatum;
	long id_target, id_relatum;
	long relations_count = 0;
	getline(relations_stm, line);
	while (relations_stm) {
		// Read the terms
		stringstream iss(line);
		if (!getline(iss, term_target, ';')) break;
		if (!getline(iss, term_relatum, ';')) break;

		// Get term ids
		id_target = _relations_vocabulary.get_word_id_ifexists(term_target);
		if(id_target == 0){
			id_target = _relations_vocabulary.add_word(term_target, true).first;
		}

		id_relatum = _relations_vocabulary.get_word_id_ifexists(term_relatum);
		if(id_relatum == 0){
			id_relatum = _relations_vocabulary.add_word(term_relatum, true).first;
		}

		//Add the relation
		if (find(id_target) == end()){
			list<long> l;// new list<long>()
			insert(make_pair(id_target, l));
		}
		find(id_target)->second.push_back(id_relatum);
		relations_count++;

		// Read next line
		getline(relations_stm, line);
	}
	printf("%ld relations were loaded\n", relations_count);
	return;
}

void TextExpander::print_id_list(list<long> id_list) {
	for (list<long>::iterator it = id_list.begin(); it != id_list.end(); it++){
	    printf("%ld; ", (*it));
	}
	printf("\n");
}

void TextExpander::print_term_list(list<string> term_list) {
	for (list<string>::iterator it = term_list.begin(); it != term_list.end(); it++){
	    printf("%s; ", (*it).c_str());
	}
	printf("\n");
}

void TextExpander::print_vector(BowVector* text_vector) {
	// For each word-id map to the term
	for (map<long,long>::iterator it = text_vector->begin(); it != text_vector->end(); it++){
		printf("%s:%ld, ", _relations_vocabulary.get_word_by_id_debug(it->first).c_str(), it->second);
	}
	printf("\n");
}

/**
 * Gets a (out of vocabulary, OOV) term and returns a list of in
 * vocabulary words semantically similarly to the OOV term.
 * */
list<long> TextExpander::get_projected_terms(string term, int max_related_terms) {
	list<long> related;
	long term_id = _relations_vocabulary.get_word_id_ifexists(term);
	if(term_id != 0){
		list<long> candidates = get_related_terms(term_id);

		list<long>::iterator it;
		int i;
		for (it = candidates.begin(), i = 0; it != candidates.end() && i < max_related_terms; it++, i++) {
			if(*it > 0){ // OOV terms have negative id
				related.push_back(*it);
			}
		}
	}
	return related;
}
