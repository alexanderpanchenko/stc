#ifndef CLASSIFIERMANAGER_HPP_
#define CLASSIFIERMANAGER_HPP_

#include "Classifier.hpp"
#include "textclassifier.h"

/**
 * This class handles several classifiers. 
 * */
class ClassifierManager {
public:
	ClassifierManager(string resources_dir, bool verbose);
	virtual ~ClassifierManager();
	bool predict(int model_id, TextToClassify* text, string output_file,
			TextExpanderParams* exp_params,	bool is_unit_length=true);
	bool predict(int model_id, TextToClassify* text, string output_file);
	bool train(int model_id, TextToClassify* text, bool is_unit_length=true);
	int add_model(TextToClassify* text, bool is_unit_length=true);
	bool reload(string resources_dir, bool verbose);
	int get_models_num();
	int* get_model_ids();
	void print_models_info();
private:
	int _max_model_id;
	bool _verbose;
	string _default_stopwords;
	string _default_stopos;
	string _default_relations;
	string _resources_dir;
	map<int, Classifier*> _classifiers;
	TextExpanderParams* _default_expansion_params;

	bool _models_loaded;
	bool string2bool(string s);
	Classifier* get(int model_id);
	bool unload_models();
	bool load_models(map<int,map<string,string> > params);
	bool load_models(string resouces_dir, bool verbose);
	int get_new_model_id();
};

#endif /* CLASSIFIERMANAGER_HPP_ */
