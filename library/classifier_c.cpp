#include "classifier_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Classifier.hpp"

void* Classifier_Create(unsigned long uliInfoMode, const char* sProgName, const char* sResourcesDir){
	// Save the parameters
	classifier_info.uliInfoMode = uliInfoMode;
	classifier_info.sProgName = sProgName;
	classifier_info.sResourcesDir = sResourcesDir;

	// Setup the paths to the resources
	string res_dir(sResourcesDir);
	string stopwords = res_dir + DEFAULT_STOPWORDS_FILE;
	string stopos = res_dir + DEFAULT_STOPOS_FILE;
	string vocabulary = res_dir + DEFAULT_VOCABULARY_FILE;
	string model = res_dir + DEFAULT_MODEL_FILE;
	string relations = res_dir + DEFAULT_RELATIONS_FILE;

	// Try to load the classifier
	if(file_exists(stopwords, false) && file_exists(stopos, false) && file_exists(vocabulary, false) && file_exists(model, false)){
		Classifier* classifier = new Classifier(stopwords, stopos, vocabulary, model, relations);
		printf("Classifier was created.\n");
		return (void*)classifier;
	}
	else{
		printf("Classifier was not created. Check that all resources exist in '%s'\n", sResourcesDir);
		return NULL;
	}

}

void Classifier_Destroy(void* vThis){
	if(vThis){
		Classifier* classifier = static_cast<Classifier*>(vThis);
		delete classifier;
		printf("Classifier was destroyed.\n");
	}
	else{
		printf("Classifier was not destroyed: nothing to destroy.\n");
	}
}


int Classifier_Reload(void* vThis, char* sErrMsg){
	Classifier_Destroy(vThis);
	if(strlen(classifier_info.sProgName)>0 &&
			file_exists(const_cast<char*>(classifier_info.sResourcesDir), false)){
		vThis = Classifier_Create(classifier_info.uliInfoMode, classifier_info.sProgName, classifier_info.sResourcesDir);
		printf("Classifier was reloaded.\n");
		return 0;
	}
	else{
		printf("Classifier can not be reloaded.\n");
		return -1;
	}
}

int Classifier_FileProcess(void* vThis, const char* sFileName, unsigned long uliInfoMode){
    if(vThis){
    	if(file_exists(const_cast<char*>(sFileName), false)){
    		string file(sFileName);
    		Classifier* classifier = static_cast<Classifier*>(vThis);
    		TextExpanderParams default_exp_params;
    		if(classifier->predict(file, &default_exp_params, true)){
    			return 0;
    		}
    		else {
    			printf("Error: file was not classified\n.");
    			return -1;
    		}

    	}
    	else{
    		printf("Error: classifier is not loaded\n.");
    		return -1;
    	}
    }
    else{
    	printf("Error: classifier is not loaded\n.");
    	return -1;
    }

    return 0;
}

int Classifier_ServerProcess(void* vThis, char* sFileName, unsigned long uliInfoMode, char* sErrMsg){
	return Classifier_FileProcess(vThis, sFileName, uliInfoMode);
}
