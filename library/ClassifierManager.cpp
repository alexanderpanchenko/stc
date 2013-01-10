#include "ClassifierManager.hpp"
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;

/**
 * Loads classifiers from a directory with resources.
 * It must contain: stopwords.csv, stopos.csv, relations.csv -- default resoures
 * Plus at least one model: 1_model, 1_vocabulary.csv.
 * Each model may contain specific resources instead of defaults (some of them or all):
 * 1_stopwords.csv, 1_stopos.csv, 1_relations.csv, 1_model, 1_vocabulary.csv
 * A directory may contain several models. Each model has a prefix "N_", where N is the
 * number of the model.
 * */
ClassifierManager::ClassifierManager(string resources_dir, bool verbose) {
	_resources_dir = resources_dir;
	_verbose = verbose;
	_models_loaded = load_models(resources_dir, verbose);
	if (!_models_loaded){
		printf("Error: loading of the models failed.");
	}
	_default_expansion_params = new TextExpanderParams();
}

ClassifierManager::~ClassifierManager() {
	unload_models();
}

/**
 * Destroy all loaded classifiers
 * */
bool ClassifierManager::unload_models() {
	map<int, Classifier*>::iterator it;
	for(it = _classifiers.begin(); it != _classifiers.end(); it++) {
		if(it->second) delete it->second;
	}
	_classifiers.clear();
	return true;
}

bool ClassifierManager::predict(int model_id, TextToClassify* text,	string output_file) {
	return predict(model_id, text, output_file, _default_expansion_params, true);
}


/**
 * Return a model id which will not conflict with the existing models
 * and files in the resources directory.
 * */
int ClassifierManager::get_new_model_id(){
	int model_id = _max_model_id + 1;
	string model_id_str;

	while(true){
		stringstream stm;
		stm << model_id;
		string model_id_str = stm.str();
		filesystem::path stopwords_path(_resources_dir + model_id_str + "_" + DEFAULT_STOPWORDS);
		filesystem::path stopos_path(_resources_dir + model_id_str + "_" + DEFAULT_STOPOS);
		filesystem::path relations_path(_resources_dir + model_id_str + "_" + DEFAULT_RELATIONS);
		filesystem::path model_path(_resources_dir + model_id_str + "_" + DEFAULT_MODEL);
		filesystem::path vocabulary_path(_resources_dir + model_id_str + "_" + DEFAULT_MODEL);
		bool model_not_exist =
				!filesystem::exists(stopwords_path) &&
				!filesystem::exists(stopos_path) &&
				!filesystem::exists(relations_path) &&
				!filesystem::exists(vocabulary_path) &&
				!filesystem::exists(model_path);

		if(model_not_exist) return model_id;
		else model_id++;
	}
}

/**
 * Add a new model to the existing ones with default parameters.
 * The model is trained on the provided input texts.
 * */
int ClassifierManager::add_model(TextToClassify* text, bool is_unit_length) {
	int model_id = get_new_model_id();
	std::stringstream stm;
	stm << model_id;
	string model_id_str = stm.str();
	string vocabulary = _resources_dir +  model_id_str + "_" + DEFAULT_VOCABULARY;
	string model = _resources_dir + model_id_str + "_" + DEFAULT_MODEL;

	Classifier* classifier = new Classifier(
		_default_stopwords, _default_stopos, vocabulary, model, _default_relations, _verbose);
	_classifiers.insert(make_pair(model_id, classifier));

	if(!train(model_id, text, is_unit_length)){
		printf("Error: the model '%d' was not trained.\n", model_id);
		return -1;
	}
	return model_id;
}

bool ClassifierManager::load_models(string resouces_dir, bool verbose){
	// Make sure that the resources exist
	_default_stopwords = resouces_dir + DEFAULT_STOPWORDS;
	_default_stopos = resouces_dir + DEFAULT_STOPOS;
	_default_relations = resouces_dir + DEFAULT_RELATIONS;

	bool default_resources_exist =
		filesystem::exists(_default_stopwords) &&
		filesystem::exists(_default_stopos) &&
		filesystem::exists(_default_relations);

	if(!default_resources_exist) {
		printf("Default recourses %s,%s, %s does not exist.\n",
				_default_stopwords.c_str(), _default_stopos.c_str(), _default_relations.c_str());
		return false;
	}

	filesystem::path p(resouces_dir);
	if(!filesystem::exists(p) || !filesystem::is_directory(p)) {
		printf("%s does not exist!", resouces_dir.c_str());
		return false;
	}

	// Create params of the models
	map<int, map<string,string> > params;
	regex stopwords_pattern("^(\\d+)_stopwords.csv$", regex_constants::icase|regex_constants::perl);
	regex stopos_pattern("^(\\d+)_stopos.csv$", regex_constants::icase|regex_constants::perl);
	regex relations_pattern("^(\\d+)_relations.csv$", regex_constants::icase|regex_constants::perl);
	regex model_pattern("^(\\d+)_model$", regex_constants::icase|regex_constants::perl);
	regex vocabulary_pattern("^(\\d+)_vocabulary.csv$", regex_constants::icase|regex_constants::perl);
	directory_iterator dir_iter(p), dir_end;
	_max_model_id = 0;

	for(;dir_iter != dir_end; ++dir_iter){
		smatch matches;
		string filename = (*dir_iter).path().filename().string();
		string file = (*dir_iter).path().string();

		// Get the key
		string key = "";
		if(regex_match(filename, matches, stopwords_pattern, match_extra)) key = string("STOPWORDS");
		else if (regex_match(filename, matches, stopos_pattern, match_extra)) key = string(KEY_STOPOS);
		else if (regex_match(filename, matches, relations_pattern, match_extra)) key = string(KEY_RELATIONS);
		else if (regex_match(filename, matches, model_pattern, match_extra)) key = string(KEY_MODEL);
		else if (regex_match(filename, matches, vocabulary_pattern, match_extra)) key = string(KEY_VOCABULARY);
		if(key == "") continue;

		// Add a key-value pair
		int model_id = atoi(string(matches[1].first, matches[1].second).c_str());
		_max_model_id = (_max_model_id < model_id ? model_id : _max_model_id);
		if(model_id >= 0){
			map<int,map<string, string> >::iterator it = params.find(model_id);
			if(it == params.end()) params.insert(make_pair(model_id, map<string,string>()));
			params[model_id].insert(make_pair(key, file));
		}
	}

	// Add defaults if needed
	map<int,map<string,string> >::iterator it_models;
	for(it_models = params.begin(); it_models != params.end(); it_models++) {
		bool stopwords_param = params[it_models->first].find(KEY_STOPWORDS) != params[it_models->first].end();
		if(!stopwords_param)
			params[it_models->first].insert(make_pair(KEY_STOPWORDS, _default_stopwords));

		bool stopos_param = params[it_models->first].find(KEY_STOPOS) != params[it_models->first].end();
		if(!stopos_param)
			params[it_models->first].insert(make_pair(KEY_STOPOS, _default_stopos));

		bool relations_param = params[it_models->first].find(KEY_RELATIONS) != params[it_models->first].end();
		if(!relations_param)
			params[it_models->first].insert(make_pair(KEY_RELATIONS, _default_relations));

		params[it_models->first].insert(make_pair(KEY_VERBOSE, (verbose ? "true" : "false")));
	}

	// Debug: Print the params
	/*
	map<int,map<string,string> >::iterator it;
	for(it = params.begin(); it != params.end(); it++) {
		map<string,string>::iterator it2;
		for(it2 = params[it->first].begin(); it2 != params[it->first].end(); it2++) {
			printf("'%d', '%d', %s, %s\n",
					it->first, params[it->first].size(),
					it2->first.c_str(), it2->second.c_str());
		}
	}
	*/

	// Load models with the params
	return load_models(params);
}

bool ClassifierManager::load_models(map<int,map<string,string> > params){
	// Load the classifiers
	map<int,map<string,string> >::iterator it_models;
	for(it_models = params.begin(); it_models != params.end(); it_models++) {
		bool stopwords_param = params[it_models->first].find(KEY_STOPWORDS) != params[it_models->first].end();
		bool stopos_param = params[it_models->first].find(KEY_STOPOS) != params[it_models->first].end();
		bool vocabulary_param = params[it_models->first].find(KEY_VOCABULARY) != params[it_models->first].end();
		bool model_param = params[it_models->first].find(KEY_MODEL) != params[it_models->first].end();
		bool relations_param = params[it_models->first].find(KEY_RELATIONS) != params[it_models->first].end();
		bool verbose_param = params[it_models->first].find(KEY_VERBOSE) != params[it_models->first].end();

		bool all_params_loaded =
				stopwords_param && stopos_param && vocabulary_param &&
				model_param && relations_param  && verbose_param;

		if(all_params_loaded){
			string stopwords = params[it_models->first][KEY_STOPWORDS];
			string stopos = params[it_models->first][KEY_STOPOS];
			string vocabulary = params[it_models->first][KEY_VOCABULARY];
			string model = params[it_models->first][KEY_MODEL];
			string relations = params[it_models->first][KEY_RELATIONS];
			string verbose_str = params[it_models->first][KEY_VERBOSE];
			bool verbose = string2bool(verbose_str);

			Classifier* classifier = new Classifier(
				stopwords, stopos, vocabulary, model, relations, verbose);
			_classifiers.insert(make_pair(it_models->first, classifier));
		}
		else{
			printf("Warning: not all %d parameters were provided for model %d. This model is not loaded.\n", INIPARAMS_NUM, it_models->first);
		}
	}

	return true;
}

bool ClassifierManager::string2bool(string s){
	if(s == "true") return true;
	else if (s == "false") return false;
	else return false;
}

Classifier* ClassifierManager::get(int model_id) {
	map<int,Classifier*>::iterator it = _classifiers.find(model_id);
	if(it != _classifiers.end()){
		return it->second;
	}
	else {
		return NULL;
	}
}

bool ClassifierManager::predict(int model_id, TextToClassify* text,
	string output_file, TextExpanderParams* exp_params, bool is_unit_length)
{
	if(!_models_loaded){
		printf("Error: no model is loaded. Load the models first.\n");
		return false;
	}

	Classifier* classifier = get(model_id);
	if(classifier){
		classifier->predict(text, output_file, exp_params, is_unit_length);
		return true;
	}
	else{
		return false;
	}
}

bool ClassifierManager::train(int model_id, TextToClassify* text, bool is_unit_length) {
	if(!_models_loaded){
		printf("Error: no model is loaded. Load the models first even if you need to train a new model. The model should be pre-loaded before training.\n");
		return false;
	}

	Classifier* classifier = get(model_id);
	if(classifier){
		classifier->train(text, is_unit_length);
		return true;
	}
	else{
		return false;
	}
}

bool ClassifierManager::reload(string resources_dir, bool verbose) {
	unload_models();
	_models_loaded = load_models(resources_dir, verbose);
	if (!_models_loaded){
		printf("Error: loading of the models failed.");
	}
	return true;
}

int ClassifierManager::get_models_num() {
	return _classifiers.size();
}

int* ClassifierManager::get_model_ids() {
	int* model_ids = new int[_classifiers.size()];
	map<int,Classifier*>::iterator it;
	int i = 0;
	for(it = _classifiers.begin(); it != _classifiers.end(); it++) {
		//printf("id[%d]=%d\n", i, it->first);
		*(model_ids + i) = it->first;
		i++;
	}
	return model_ids;
}

void ClassifierManager::print_models_info() {
	map<int,Classifier*>::iterator it;
	for(it = _classifiers.begin(); it != _classifiers.end(); it++) {
		const char* is_loaded = (it->second->model_is_loaded() ? "true" : "false");
		printf("ModelID=%d\nTrained=%s\nStopwords=%s\nStopos=%s\nRelations=%s\nVocabulary=%s\nModel=%s\n\n",
			it->first, is_loaded, it->second->get_stopwords_file().c_str(),
			it->second->get_stopos_file().c_str(), it->second->get_relations_file().c_str(),
			it->second->get_vocabulary_file().c_str(), it->second->get_model_file().c_str());
	}
}
