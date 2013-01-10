
/*
void foo_extract() {

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
}

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
