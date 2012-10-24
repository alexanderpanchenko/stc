#include "FreqVocabulary.hpp"

FreqVocabulary::FreqVocabulary(bool verbose) {
	VERBOSE = verbose;
	if (VERBOSE)
		printf("Vocabulary was constructed\n");
}

FreqVocabulary::FreqVocabulary(string vocFile, bool verbose) {
	VERBOSE = verbose;
	if (!load(vocFile))
		cout << "Error: can not load file " << vocFile << endl;
}

/**
 * Loads vocabulary from a CSV file "term;id;frequency;"
 * */
bool FreqVocabulary::load(string voc_file) {
	if (VERBOSE)
		printf("Loading of vocabulary file: %s...\n", voc_file.c_str());

	// Open the file
	ifstream voc_stm;
	voc_stm.open(voc_file.c_str());
	if (!voc_stm)
		return false;
	string line;
	string term;
	string tmp;
	long id;
	long freq;

	getline(voc_stm, line);
	while (voc_stm) {
		// Parse contents of the line
		stringstream iss(line);

		// Read term
		if (!getline(iss, term, ';')) break;

		// Read id
		if (!getline(iss, tmp, ';')) break;
		id = atoi(tmp.c_str());

		// Read frequency
		if (!getline(iss, tmp, ';')) break;
		freq = atoi(tmp.c_str());

		//if (VERBOSE) cout << term << "," << id << "," << freq << endl;

		// Add an entry
		this->insert(make_pair(term, make_pair(id, freq)));

		// Read next line
		getline(voc_stm, line);
	}

	if(VERBOSE) printf("%d terms were loaded\n", this->size());
	return true;
}

bool FreqVocabulary::save(string voc_file) {
	if (VERBOSE)
		printf("Saving vocabulary to file: %s...\n", voc_file.c_str());

	// Open the output file
	FILE* f = fopen(voc_file.c_str(), "w");
	if (!f)
		return false;

	// Write all entries to the file
	map<string, pair<long, long> >::iterator it;
	for (it = this->begin(); it != this->end(); it++) {
		fprintf(f, "%s;%d;%d;\n", (*it).first.c_str(), (*it).second.first,
				(*it).second.second);
	}

	fclose(f);

	return true;
}

/**
 * Returns ID of the input word. If the word does not exist
 * it will be added to the vocabulary.
 * Warning: This method would always return id. If word does not
 * exist then a new id will be returned. If yon only need to return
 * words in the dictionary then use get_word_id_ifexist.
 * */
long FreqVocabulary::get_word_id(string word) {
	return get_word_info(word).first;
}

bool FreqVocabulary::exists(string word) {
	map<string, pair<long, long> >::iterator it = this->find(word);
	return it != this->end();
}

/**
 * Returns ID of the input word only if it exists in the vocabulary.
 * Otherwise returns 0.
 * */
long FreqVocabulary::get_word_id_ifexists(string word) {
	map<string, pair<long, long> >::iterator it;
	it = this->find(word);
	if (it != this->end()) {
		pair<long, long> info = (*it).second;
		return info.first;
	}
	else {
		return 0;
	}
}

pair<long, long> FreqVocabulary::get_word_info(string word) {
	pair<long, long> info;

	// Try to find existing entry
	map<string, pair<long, long> >::iterator it;
	it = this->find(word);
	if (it != this->end()) {
		info = (*it).second;
	}
	// Create new entry
	else {
		info = add_word(word);
	}

	return info;
}

void FreqVocabulary::print_contents(void) {
	if (VERBOSE)
		printf("Contents of the vocabulary:");
	map<string, pair<long, long> >::iterator it;
	for (it = this->begin(); it != this->end(); it++) {
		cout << it->first << ";" << it->second.first << ";" << it->second.second
				<< endl;
	}
	cout << "Contains " << this->size() << " elements" << endl;
}

/**
 * Returns word by its identifier. Warning: the method was designed for
 * debug purposes and works slow.If word not found then "?" is returned.
 * */
string FreqVocabulary::get_word_by_id_debug(long id) {
	map<string, pair<long,long> >::const_iterator it;
	for (it = this->begin(); it != this->end(); ++it){
		if (it->second.first == id){
			return it->first;
		}
	}
	return "?";
}


pair<long, long> FreqVocabulary::add_word(string word, bool negative_id) {
	long id = (long) this->size() + 1;
	if(negative_id) id = -id;
	long freq = 1;
	pair<long, long> info = make_pair(id, freq);
	this->insert(make_pair(word, info));

	return info;
}

bool FreqVocabulary::increment_word(string word) {
	map<string, pair<long, long> >::iterator it;
	it = this->find(word);
	if (it != this->end()) {
		(*it).second.second++;
	}
	// Create new entry
	else {
		add_word(word);
	}

	return true;
}

FreqVocabulary::~FreqVocabulary() { }

void vocabulary_test(void) {
	FreqVocabulary* voc = new FreqVocabulary();

	string w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);

	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "b";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "c";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "d";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "e";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "a";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "w";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "b";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	w1 = "e";
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	voc->increment_word(w1);
	voc->save("/home/sasha/work/data/test/filenames-voc.csv");

	delete voc;

	voc = new FreqVocabulary();
	voc->load("/home/sasha/work/data/test/filenames-voc.csv");

	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);
	printf("%s id=%d, freq=%d\n", w1.c_str(), voc->get_word_id(w1),
			voc->get_word_info(w1).second);

	voc->print_contents();
}


