#include "Classifier.hpp"

Classifier::Classifier(string stopwords_file, string stopos_file,
		string vocabulary_file, string model_file, string relations_file, bool verbose)
{
	// Save the resources of the model
	this->_verbose = verbose;
	this->_stopwords_file = stopwords_file;
	this->_stopos_file = stopos_file;
	this->_vocabulary_file = vocabulary_file;
	this->_model_file = model_file;
	this->_relations_file = relations_file;

	// Initialize the data
	this->_stopwords = NULL;
	this->_stopos = NULL;
	this->_vocabulary = NULL;
	this->_model = NULL;
	this->_expander = NULL;
	this->_model_is_loaded = false;

	// Load resources required for training
	bool training_resources_exist =
			file_exists(stopwords_file.c_str(), false) &&
			file_exists(stopos_file.c_str(), false) &&
			file_exists(relations_file.c_str(), false);
	if(training_resources_exist) load_train_resources();
	else return;

	// Load resources required for prediction
	bool prediction_resources_exist =
			file_exists(vocabulary_file.c_str(), false) &&
			file_exists(model_file.c_str(), false);
	if(prediction_resources_exist) load_predict_resources();
	if(_verbose) printf("Classifier was loaded.\n");

}

bool Classifier::load_train_resources(){
	this->_stopwords = new Vocabulary(this->_stopwords_file, _verbose);
	this->_stopos = new Vocabulary(this->_stopos_file, _verbose);
	return true;
}

bool Classifier::load_predict_resources(){
	// Load the resources
	this->_vocabulary = new FreqVocabulary(this->_vocabulary_file, _verbose);
	this->_model = load_model(this->_model_file.c_str());
	this->_expander = new TextExpander(this->_relations_file, *(this->_vocabulary), _verbose);

	if(this->_vocabulary->size() <= 2) printf("Warning: Vocabulary contains only two or less words.\n");
	this->_model_is_loaded = true;
	return true;
}

Classifier::~Classifier() {
	// Free resources
	if(this->_stopos != NULL) delete this->_stopos;
	if(this->_stopwords != NULL) delete this->_stopwords;
	unload_predict_resources();
}

bool Classifier::model_is_loaded() {
	return _model_is_loaded;
}

string Classifier::get_stopwords_file() {
	return _stopwords_file;
}

string Classifier::get_stopos_file() {
	return _stopos_file;
}

string Classifier::get_vocabulary_file() {
	return _vocabulary_file;
}

string Classifier::get_model_file() {
	return _model_file;
}

string Classifier::get_relations_file() {
	return _relations_file;
}

void Classifier::unload_predict_resources(){
	if(this->_vocabulary != NULL) delete this->_vocabulary;
	if(this->_expander != NULL) delete this->_expander;
	if(this->_model != NULL) {
		free_and_destroy_model(&_model);
		this->_model = NULL;
	}
}

/*
 * Loads text(s) from an XML file to memory.
 * */
TextToClassify* Classifier::load_texts(string input_fpath) {
	if(!file_exists(input_fpath, false)){
		printf("Error: input XML file '%s' doesn't exist!\n", input_fpath.c_str());
		return NULL;
	}

	// Read XML file to memory
	string xml_string;
	ifstream xml_file;
	xml_file.open(input_fpath.c_str());
	string line;
	getline(xml_file,line);
	while (xml_file) {
		xml_string += line + "\n";
		getline(xml_file,line);
	}
	if (xml_string.length() <= 0){
		printf("Error: Input XML file %s is void!\n", input_fpath.c_str());
		return NULL;
	}

	// Parse XML content
	char* xml_text = (char*)xml_string.c_str();
	xml_document<> document;
	document.parse<0>(xml_text);
	//regex remove_parse_pattern("[^ ]+#[^# ]+#", regex_constants::icase|regex_constants::perl);
	//regex space_token_pattern("\\s+#[^# ]+#\\s+", regex_constants::icase|regex_constants::perl);
	regex whitespace_pattern("\\s+");

	TextToClassify* text_beg = 0;
	TextToClassify* text_prev = 0;
	TextToClassify* text_cur = 0;
	for (xml_node<>* node = document.first_node(ROOT_TAG)->first_node(TEXT_TAG); node; node = node->next_sibling()) {
		// Create a structure for current XML element
		text_cur = create_text();

		// Read class attribute
		xml_attribute<> *attr = node->first_attribute(CLASS_ATT);
		string label(attr->value());
		if(label == POSITIVE_CLASS_ATT) text_cur->cClass = POSITIVE_CLASS_I;
		else if(label == NEGATIVE_CLASS_ATT) text_cur->cClass = NEGATIVE_CLASS_I;
		else{
			printf("Wrong  class attribute value %s\n", label.c_str());
			continue;
		}

		// Load original string
		char* filename_original = node->first_node(ORIGINAL_TAG)->value();
		text_cur->sText = new char[strlen(filename_original)+1];
		sprintf(text_cur->sText, "%s", filename_original);

		// Load tokens
		string tokens_str(node->first_node(LEMMAS_TAG)->value());
		sregex_token_iterator token_it(tokens_str.begin(), tokens_str.end(), whitespace_pattern, -1);
		sregex_token_iterator end_it;
		TokenToClassify* token_cur = 0;
		TokenToClassify* token_prev = 0;
		string token;
		while(token_it != end_it) {
			// Create a token
			token_cur = create_token();
			token = *token_it;

			// Fill the token with data
			stringstream iss(token);
			string tmp;
			if (!getline(iss, tmp, '#')) {token_it++; continue;}
			algorithm::to_lower(tmp);
			token_cur->sSurface = new char[strlen(tmp.c_str())+1];
			sprintf(token_cur->sSurface, "%s", tmp.c_str());

			if (!getline(iss, tmp, '#')) {token_it++; continue;}
			token_cur->sPOS = new char[strlen(tmp.c_str())+1];
			sprintf(token_cur->sPOS, "%s", tmp.c_str());

			if (!getline(iss, tmp, '#')) {token_it++; continue;}
			algorithm::to_lower(tmp);
			token_cur->sLemma = new char[strlen(tmp.c_str())+1];
			sprintf(token_cur->sLemma, "%s", tmp.c_str());

			// Save the token
			if(text_cur->pToken == NULL) text_cur->pToken = token_cur; // First token -> previous is null
			else token_prev->pNext = token_cur; // Not first token -> set previous
			token_prev = token_cur;

			// Move to the next token
			token_it++;
		}


		////////////////////////////////////
		/*
		// Add a fake token to the end
		token_cur = create_token();
		string tmp;
		tmp = " ";
		token_cur->sSurface = new char[strlen(tmp.c_str())+1];
		sprintf(token_cur->sSurface, "%s", tmp.c_str());

		tmp = "NN";
		token_cur->sPOS = new char[strlen(tmp.c_str())+1];
		sprintf(token_cur->sPOS, "%s", tmp.c_str());

		tmp = " ";
		token_cur->sLemma = new char[strlen(tmp.c_str())+1];
		sprintf(token_cur->sLemma, "%s", tmp.c_str());

		token_prev->pNext = token_cur;
		*/
		////////////////////////////////////


		// Set a link between previous and current structures
		if(text_beg == NULL) text_beg = text_cur;
		else text_prev->pNext = text_cur;
		text_prev = text_cur;
	}

	return text_beg;
}

/**
 * Creates a new instance of ClassifierToken
 * */
TokenToClassify* Classifier::create_token(){
	TokenToClassify* ct = new TokenToClassify();
	ct->sSurface = 0;
	ct->sPOS = 0;
	ct->sLemma = 0;
	ct->pNext = NULL;
	return ct;
}

/**
 * Creates a new instance of ClassifierToken
 * */
TokenToClassify* Classifier::create_token(char* surface, char* pos, char* lemma){
	TokenToClassify* ct = new TokenToClassify();
	ct->sSurface = surface;
	ct->sPOS = pos;
	ct->sLemma = lemma;
	ct->pNext = NULL;
	return ct;
}

/**
 * Creates a new instance of ClassifierText
 * */
TextToClassify* Classifier::create_text(){
	TextToClassify* ct = new TextToClassify();
	ct->sText = 0;
	ct->pToken = NULL;
	ct->pNext = NULL;
	ct->cClass = 0;
	return ct;
}

/**
 * Transforms a list of texts into a vectors saved in output_fpath in SVMLIB format,
 * according to the vocabulary of the current classifier.
 * */
list<bool> Classifier::text2vector(TextToClassify* text, string output_fpath,
		TextExpanderParams* exp_params, bool is_unit_length) {
	// Initialization
	list<bool> has_feature_list;
	FILE* output_file = fopen(output_fpath.c_str(), "w");
	if(!_model_is_loaded) this->_vocabulary = new FreqVocabulary();
	int count = 0;
	TokenToClassify* token;
	BowVector* features;
	FreqVocabulary relation_voc = _expander->get_vocabulary(); // Debug
	// Read texts
	while(text){
		//printf("\n\nText:%s\nFeatures:", text->sText); // Debug
		// Convert tokens to a numerical vector
		token = text->pToken;
		features = new BowVector();
		int token_id;

		while(token){
			printf("'%s', ", token->sLemma); // Debug
			if(token->sLemma && !skip_token(token->sLemma)) {
				string lemma(token->sLemma);
				if(!_model_is_loaded){
					// Add the token whether it is in vocabulary or not
					token_id = _vocabulary->get_word_id(lemma);
					features->increment_word(token_id);
					_vocabulary->increment_word(lemma);
				}
				else if(_vocabulary->exists(lemma)){
					// Add token if it is in the vocabulary
					token_id = _vocabulary->get_word_id_ifexists(lemma);
					features->increment_word(token_id);
				}
				else if(exp_params->max_expansions > 0){
					// Add related words
					list<long> projected_terms = _expander->get_projected_terms(lemma,  exp_params->max_expansions);
					for(list<long>::iterator it = projected_terms.begin(); it != projected_terms.end(); it++) {
						features->increment_word(*it);
					}

					// Debug
					cout << "expand" << endl;
					if(projected_terms.size() > 0) cout << lemma << ":" << relation_voc.get_word_by_id_debug(*(projected_terms.begin())) << ":" << projected_terms.size() << endl;
					cout << "\n>>>" << lemma << " = ";
					for(list<long>::iterator it = projected_terms.begin(); it != projected_terms.end(); it++) {
						cout << *it << ":" << relation_voc.get_word_by_id_debug(*it) << ", ";
					}
					cout << "." << endl;
				}
			}
			token = token->pNext;
		}

		// Expand the feature vector
		switch (exp_params->type) {
		case NO_EXP:
			// No expansion
			break;
		case FIRST_ORDER_EXP:
			features = _expander->enrich_vector(features, exp_params->max_expansions, exp_params->expansion_weight, false);
			break;
		case SECOND_ORDER_EXP:
			features = _expander->enrich_vector(features, exp_params->max_expansions, exp_params->expansion_weight, true);
		  	break;
		case RECURSIVE_EXP:
			features = _expander->enrich_vector_recursive(features, exp_params->max_expansions, exp_params->expansion_weight);
			break;
		default :
			// No expansion
			break;
		}

		// Save vector to the file
		bool has_features = features->size() >= MIN_FEATURES_NUM;
		if(has_features || _model_is_loaded){
			has_feature_list.push_back(has_features);
			fprintf(output_file, "%d %s\n", text->cClass, features->to_string(is_unit_length).c_str());
			count++;
		}

		// Next text
		text = text->pNext;
		delete features;
	}

	// Save the files
	if(!_model_is_loaded) {
		this->_vocabulary->save(this->_vocabulary_file);
		delete this->_vocabulary;
	}
	fclose(output_file);
	//string s1(output_file); s1 = s1 + ".reconstr";reconstruct(output_file, s1.c_str() , s.c_str());
	printf("\nfeatures were extracted from %d texts.\n", count);
	return has_feature_list;
}

/**
 * A debug method. Takes as input a vector and reconstructs it according to a given vocabulary.
 * The reconstructed vector is printed to the output.
 * */
void Classifier::reconstruct_vector(string input_vector, string output_texts, string vocabulary_file) {
	// Initialization
	FreqVocabulary* voc = new FreqVocabulary(vocabulary_file.c_str());
	FILE* f = fopen(output_texts.c_str(), "w");
	if (!f){
		printf("Can not open file %s\n", output_texts.c_str());
		return;
	}
	regex remove_parse_pattern(":\\d+\\.\\d+", regex_constants::icase|regex_constants::perl);
	regex remove_parse_pattern2("-1\\s+", regex_constants::icase|regex_constants::perl);
	regex remove_parse_pattern3("\\+1\\s+", regex_constants::icase|regex_constants::perl);
	regex whitespace_pattern("\\s+");
	ifstream vectors_file;
	vectors_file.open(input_vector.c_str());
	string line;

	// Read vector lines
	getline(vectors_file,line);
	while (vectors_file) {
		//Tokenize the vector line
		string filename;
		filename = regex_replace(line, remove_parse_pattern, "");
		filename = regex_replace(filename, remove_parse_pattern2, "");
		filename = regex_replace(filename, remove_parse_pattern3, "");

		sregex_token_iterator token_it(filename.begin(), filename.end(), whitespace_pattern, -1);
		sregex_token_iterator end_it;

		// Write tokens of the line
		while(token_it != end_it) {
			string token = *token_it;
			long id = strtol(token.c_str(),NULL,10);
			fprintf(f, "%s:%d ", voc->get_word_by_id_debug(id).c_str(), id);
			token_it++;
		}

		fprintf(f, "\n");
		getline(vectors_file,line);
	}

	fclose(f);
	delete voc;
}

list<bool> Classifier::extract(string input_file, string output_file,
		TextExpanderParams* exp_params, bool is_unit_length) {

	TextToClassify* text = load_texts(input_file);
	list<bool> has_feature_list = text2vector(text, output_file, exp_params, is_unit_length);
	delete_text(text);
	return has_feature_list;
}

bool Classifier::train(TextToClassify* text, bool is_unit_length){
	if(_verbose) printf("Training...\n");

	// Check if input data exist
	if(!text){
		printf("Error: no training data.\n");
		return false;
	}

	// Destroy existing model
	this->_model_is_loaded = false;
	unload_predict_resources();

	// Train the model
	string vectors_file = this->_model_file + ".csv";
	TextExpanderParams* default_exp_params = new TextExpanderParams();
	list<bool> has_data = text2vector(text, vectors_file, default_exp_params, is_unit_length);
	if(has_data.size() == 0){
		printf("Error: cannot extract features training data. \n");
		return false;
	}

	train_fs(vectors_file.c_str(), this->_model_file.c_str());
	delete default_exp_params;

	// Load the new model
	load_predict_resources();
	this->_model_is_loaded = true;

	// Remove temporaty files
	if(DELETE_TMP){
		wpath vectors_path(vectors_file);
		if(exists(vectors_path)) remove(vectors_path);
	}

	return true;
}

/**
 * Trains a new classification model from the training data and output a classification model.
 * in_training_file 	Path to an XML filename file with training instances
 * out_model_file 		Path to the output file with the model
 * out_vocabulary_file 	Path to the output vocabulary file
 * is_unit_length		Unit norm normalization of the feature vectors
 * */
bool Classifier::train(string input_file, bool is_unit_length) {
	if(_verbose) printf("Training...\n");

	if(!file_exists(input_file, false)){
			printf("Error: Training file '%s' not found!\n", input_file.c_str());
			return false;
	}

	// Load training data
	TextToClassify* texts = load_texts(input_file);
	if(!texts){
		printf("Error: cannot load text from XML file '%s'\n", input_file.c_str());
		return false;
	}

	// Train the model and save it to this->_model_file
	bool result = train(texts, is_unit_length);

	// Delete the training data
	delete_text(texts);

	return result;
}

bool Classifier::predict(TextToClassify* texts, string output_file, TextExpanderParams* exp_params,	bool is_unit_length) {
	if(_verbose) printf("Predicting...\n");

	if(!_model_is_loaded){
		printf("Error: model is not loaded. Train a model first.\n");
		return false;
	}

	if(!texts){
		printf("Error: no texts to classify.\n");
		return false;
	}

	string vectors_file = output_file + ".csv";
	list<bool> has_data = text2vector(texts, vectors_file, exp_params, is_unit_length);
	if(has_data.size() == 0){
		printf("Error: cannot extract features from texts.\n");
		return false;
	}

	string predict_file = output_file + ".predict";
	list<int> labels = predict_fs(vectors_file.c_str(), predict_file.c_str(), this->_model);
	text2xml(texts, has_data, labels, output_file);

	if(DELETE_TMP){
		wpath vectors_path(vectors_file);
		if(exists(vectors_path)) remove(vectors_path);

		wpath predict_path(predict_file);
		if(exists(predict_path)) remove(predict_path);
	}
}

/**
 * The difference from other file-based predict is that the original file is not modified
 * */
bool Classifier::predict(string input_file, string output_file, TextExpanderParams* exp_params, bool is_unit_length){
	if(_verbose) printf("Predicting...\n");

	TextToClassify* texts = load_texts(input_file);
	if(!texts){
		printf("Error: cannot load text from XML file '%s'\n", input_file.c_str());
		return false;
	}

	bool result = predict(texts, output_file, exp_params, is_unit_length);
	delete_text(texts);

	return result;
}


bool Classifier::foo(TextToClassify* text, list<bool> hasdata, list<int> labels, string output_fpath){
	return true;

}

/**
 * Predicts values of the new data file. The output classification is saved
 * into the same file (as xml attributes). The predicted values are saved
 * into the file "in_data_file.predict"
 * in_data_file 	An XML filename file with input filenames
 * is_unit_length 	If yes then vectors will be L1-normalized
 * */
bool Classifier::predict(string input_file, TextExpanderParams* exp_params,  bool is_unit_length) {
	if(_verbose) printf("Predicting...\n");

	// Load texts
	TextToClassify* texts = load_texts(input_file);
	if(!texts){
		printf("Error: cannot load text from XML file '%s'\n", input_file.c_str());
		return false;
	}

	// Extract features from the text
	string vectors_file = input_file + ".csv";
	list<bool> has_data = text2vector(texts, vectors_file, exp_params, is_unit_length);
	if(has_data.size() == 0){
		printf("Error: cannot extract features from %s\n", input_file.c_str());
		return false;
	}

	// Predict labels of the texts
	string predict_file = input_file + ".predict";
	list<int> labels = predict_fs(vectors_file.c_str(), predict_file.c_str(), this->_model);

	// Delete input file
	string input_file_copy(input_file); // Bug: string destroyed after remove
	wpath input_path(input_file);
	if(exists(input_path)) remove(input_path);
	text2xml(texts, has_data, labels, input_file_copy);
	//insert_labels(input_file, labels, has_data); // The old version -- instead of four lines above

	if(DELETE_TMP){
		wpath vectors_path(vectors_file);
		if(exists(vectors_path)) remove(vectors_path);
		wpath predict_path(predict_file);
		if(exists(predict_path)) remove(predict_path);
	}

	// Delete texts
	delete_text(texts);

	return true;
}

/**
 * The save an instance of TextToClassify structure in an XML file.
 * The method use the class labels, and has_data lists in order to
 * assign class labels {-1, 0, +1}
 * */
bool Classifier::text2xml(TextToClassify* text, list<bool> hasdata,
		list<int> labels, string output_fpath) {

	// Get the number of texts
	TextToClassify* first_text = text;
	int texts_count = 0;
	bool texts_has_no_id = true;
	while(text){
		texts_has_no_id = texts_has_no_id && (text->iID == 0);
		text = text->pNext;
		texts_count++;
	}
	//printf("#hasdata=%d, #labels=%d, #texts=%d\n", hasdata.size(), labels.size(), texts_count);

	// Data are consistent -> write output
	if((hasdata.size() == texts_count) && (labels.size() == texts_count)){

		// Open output file
		FILE* output_file = fopen(output_fpath.c_str(), "w");
		if (!output_file){
			printf("Error: Cannot open the file '%s'.\n", output_fpath.c_str());
			return false;
		}

		// Write each text as an XML element
		const char* original_text;
		string lemmas_text;
		int label;
		string confidence;
		int text_num = 1;
		text = first_text;
		list<int>::iterator labels_it = labels.begin();
		list<bool>::iterator hasdata_it = hasdata.begin();

		fprintf(output_file, "<%s>\n", ROOT_TAG);
		while(text && labels_it != labels.end() && hasdata_it != hasdata.end()){
			label = (*hasdata_it ? *labels_it : NEGATIVE_CLASS_I);
			confidence = (*hasdata_it ? DEFAULT_CONFIDENCE_ATT : DEFAULT_UNKNOWN_CONFIDENCE_ATT );
			fprintf(output_file, "<%s %s='%d' %s='%s' %s='%s'>\n",
					TEXT_TAG, ID_ATT, (texts_has_no_id ? text_num : text->iID),
					CLASS_ATT, get_label_name(label).c_str(),
					CONFIDENCE_ATT, confidence.c_str());

			original_text = (text->sText ? text->sText : "");
			fprintf(output_file, "<%s>%s</%s>\n", ORIGINAL_TAG, original_text, ORIGINAL_TAG);

			lemmas_text = (text->pToken ? token2string(text->pToken) : "");
			fprintf(output_file, "<%s>%s</%s>\n", LEMMAS_TAG, lemmas_text.c_str(), LEMMAS_TAG);

			fprintf(output_file, "</%s>\n", TEXT_TAG);

			// Next text
			text = text->pNext;
			labels_it++;
			hasdata_it++;
			text_num++;
		}
		fprintf(output_file, "</%s>", ROOT_TAG);

		// Close output file
		fclose(output_file);

		return true;
	}
	// Input data are not consistent
	else{
		printf("Error: Input data are not consistent. Output file was not written.\n");
		return false;
	}
}

static const boost::regex has_number_exp("\\d+");

/**
 * Returns true is this tokens should not be used as a feature because
 * 1) it's a stop word
 * 2) its POS is in stop pos list
 * 3) it is a number or a mix of number and letters
 * */
bool Classifier::skip_token(string token) {
	bool is_a_stopword = this->_stopwords->contains(token);
	//static const boost::regex is_number_exp("[\\s.,]*\\d+[\\s.]*\\d*");
	//bool is_a_number = boost::regex_(token, is_number_exp);
	//static const boost::regex has_number_exp("\\d+");
	bool has_number = boost::regex_search(token, has_number_exp);
	return is_a_stopword || has_number;
}

/**
 * This method generates a test feature node.
 * */
feature_node* Classifier::generate_feature_node(int size) {
	struct feature_node* vector;
	vector = (struct feature_node *) realloc(vector, size*sizeof(struct feature_node));
	for(int i=0; i<size; i++){
		vector[i].index = i;
		vector[i].value = 0.33;
	}
	return vector;
}

/**
 * Deletes a token.
 * */
void Classifier::delete_token(TokenToClassify* token) {
	TokenToClassify* next_token;

	while(token){
		if(token->sLemma) delete[] token->sLemma;
		if(token->sPOS) delete[] token->sPOS;
		if(token->sSurface) delete[] token->sSurface;
		next_token = token->pNext;
		delete token;
		token = next_token;
	}
}

/**
 * Creates an textual representation of a list of tokens
 * in the format "surface#POS#lemma ..."
 * */
string Classifier::token2string(TokenToClassify* token) {
	string result = "";
	while(token){
		if(token->sLemma && token->sPOS && token->sSurface){
			result += token->sSurface;
			result += TOKEN_SEPARATOR;
			result += token->sPOS;
			result += TOKEN_SEPARATOR;
			result += token->sLemma;
			result += " ";
		}
		token = token->pNext;
	}
	return result;
}

/**
 * Deletes a text.
 * */
void Classifier::delete_text(TextToClassify* text) {
	TextToClassify* next_text;

	while(text){
		if(text->sText) delete[] text->sText;
		if(text->pToken) delete_token(text->pToken);
		next_text = text->pNext;
		delete text;
		text = next_text;
	}
}

/*
bool Classifier::insert_labels(string xml_fpath, list<int> labels, list<bool> has_data) {
	// Get XML
	string xml_string;
	ifstream xml_file;
	xml_file.open(xml_fpath.c_str());
	string line;
	getline(xml_file,line);
	while (xml_file) {
		xml_string += line + "\n";
		getline(xml_file,line);
	}
	char* xml_text = (char*)xml_string.c_str();
	xml_document<> document;
	document.parse<0>(xml_text);
	xml_file.close();

	// Read the XML
	list<int>::iterator label = labels.begin();
	list<bool>::iterator data = has_data.begin();
	int xml_size = 0;
	for (xml_node<>* node = document.first_node(ROOT_TAG)->first_node(TEXT_TAG); node; node = node->next_sibling()) {
		// Set the attribute
	 	if(*label == POSITIVE_CLASS_I && *data == true){
	 		node->first_attribute(CLASS_ATT)->value(POSITIVE_CLASS_ATT);
	 	}
	 	else if (*label == NEGATIVE_CLASS_I && *data == true){
	 		node->first_attribute(CLASS_ATT)->value(NEGATIVE_CLASS_ATT);
	 	}
	 	else if(*data == false){
	 		node->first_attribute(CLASS_ATT)->value(UNKNOWN_CLASS_ATT);
	 	}

	 	//printf("%s %d %d\n", attr->value(), *i, *j); // Debug
	 	label++;
	 	data++;
	 	xml_size++;
	}

	// Check that the data are consistent
	bool has_inconsistency = (labels.size() != has_data.size() || labels.size() != xml_size);
	if(has_inconsistency){
		printf("Inconsistency in the prediction results\n.");
		return false;
	}
	else{
		// Save the XML
		string s;
		print(back_inserter(s), document, 0);
		ofstream ofFileToWrite;
		ofFileToWrite.open(xml_fpath.c_str(), ios::trunc);
		ofFileToWrite << "<?xml version=\"1.0\"?>\n" << s ;
		ofFileToWrite.close();
		return true;
	}
}
*/




