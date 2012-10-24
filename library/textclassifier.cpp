#include "textclassifier.h"
#include "ClassifierManager.hpp"

void* TextClassifier_Create(char* sResourcesDir, int bVerbose){
	ClassifierManager* cm = new ClassifierManager(string(sResourcesDir), (bVerbose ? true : false));
	printf("TextClassifier was created.\n");
	return (void*)cm;
}

void TextClassifier_Destroy(void* vThis){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		delete cm;
		printf("TextClassifier was destroyed.\n");
	}
	else{
		printf("TextClassifier was not destroyed: nothing to destroy.\n");
	}
}

int TextClassifier_Add(void* vThis, TextToClassify* text){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		return cm->add_model(text, true);
	}
	else{
		printf("TextClassifier does not exist.\n");
		return -1;
	}
}

void TextClassifier_Reload(void* vThis, char* sResourcesDir, int bVerbose){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		cm->reload(string(sResourcesDir), (bVerbose ? true : false));
		printf("TextClassifier was reloaded.\n");
	}
	else{
		printf("TextClassifier does not exist.\n");
	}
	return;
}

void TextClassifier_Predict(void* vThis, const char* sOut, int nModel, TextToClassify* pTextToCls){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		cm->predict(nModel, pTextToCls, sOut);
	}
	else{
		printf("TextClassifier does not exist.\n");
	}
	return;
}

void TextClassifier_Train(void* vThis, int nModel, TextToClassify* pTextToTrain){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		cm->train(nModel, pTextToTrain, true);
	}
	else{
		printf("TextClassifier does not exist.\n");
	}
	return;
}

void TextClassifier_PrintInfo(void* vThis){
	if(vThis){
		ClassifierManager* cm = static_cast<ClassifierManager*>(vThis);
		cm->print_models_info();
	}
	else{
		printf("TextClassifier does not exist.\n");
	}
	return;
}
