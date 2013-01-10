#include "Vocabulary.hpp"

Vocabulary::Vocabulary(string file, bool verbose) {
	VERBOSE = verbose;
	if(!load(file))
		printf("Error: vocabulary file %s was not loaded correctly.", file.c_str());
}

Vocabulary::~Vocabulary() { }

/**
 * Loads vocabulary from a CSV file "term\n"
 * */
bool Vocabulary::load(string voc_file) {
	if (VERBOSE) printf("Loading vocabulary from file: %s... \n", voc_file.c_str());
	// Open the file
	ifstream voc_stm;
	voc_stm.open(voc_file.c_str());
	if (!voc_stm) return false;

	string term;
	long id;

	getline(voc_stm, term);
	while (voc_stm) {
		// Add an entry
		id = size();
		insert(make_pair(term, id));
		//if (globalArgs.verbose) cout << term << "," << id << endl;

		// Read next line
		getline(voc_stm, term);
	}

	if (VERBOSE) printf("%d words were loaded\n", size());
	return true;
}

/**
 * Check if the word is in the vocabulary
 * */
bool Vocabulary::contains(string word) {
	map<string, long>::iterator it = find(word);
	return (it != end());
}

/**
 * Finds ID of a word
 * */
long Vocabulary::get_word_id(string word) {
	map<string, long>::iterator it = find(word);
	if (it != end()) return (*it).second;
	else return -1;
}

void test_simple_vocabulary(void) {
	Vocabulary* voc = new Vocabulary("/home/sasha/work/data/test/stopwords.csv");
	cout << voc->contains("the") << endl;
	cout << voc->contains(".") << endl;
	cout << voc->contains(")") << endl;
	cout << voc->contains(",") << endl;

	delete voc;
}
