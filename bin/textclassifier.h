/**
 * This is the C interface of the text categorization library (libsimple_text_classifier.so).
 * */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "iniparam.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A structure serving as input for Classifier_Process:
 Classifier_Process(TextToClassify* pText);
 All the fields are required.*/

typedef struct _TextToClassify  TextToClassify;
typedef struct _TokenToClassify TokenToClassify;

struct _TextToClassify {
	char* sText;              /* Contents of the sOut file -- original text BEFORE any preprocessing */
	TokenToClassify* pToken;  /* A list of tokens -- preprocessed version of sOut text */
    TextToClassify* pNext;
    char cClass; /* -1, 0, +1 */
    long iID; /* MUST equal to 0 if no ID is assigned */
    double dConfidence; /* Between 0 and 1, default 0.9 */
};

struct _TokenToClassify {
	char* sSurface;         /* Surface form of a token e.g. "proved" */
	char* sPOS;             /* A disambiguated part of speech of a token e.g. "VVN" */
	char* sLemma;           /* Lemma of the token e.g. "prove" */
	TokenToClassify* pNext; /* A pointer to the next token. If null then the last token. */
};

/**
 * Performs initialization of the classifier. Several models may be initialized at the same time.
 * Returns a pointer to the text classifier.
 *  */
void* TextClassifier_Create(char* sResourcesDir, int bVerbose);

/**
 * Destroys a text classifier. Unloads all the loaded models.
 * */
void TextClassifier_Destroy(void* vThis);

/**
 * Reloads the models of an existing text classifier.
 * */
void TextClassifier_Reload(void* vThis, char* sResourcesDir, int bVerbose);

/**
 * Performs a classification of a new text represented in a structure pTextToCls.
 * nModel is the number of the model N, as defined in IniParam structure passed
 * to  TextClassifier_Create.
 * */
void TextClassifier_Predict(void* vThis, const char* sOut, int nModel, TextToClassify* pTextToCls);


/**
 * Performs training of a new model. Add a new model to the existing ones with default
 * parameters. The model is trained on the provided input texts.
 * If -1 is returned then error is occured and a model was not trained.
 * Otherwise a model_id is returned. It should be used to access the model.
 * */
int TextClassifier_Add(void* vThis, TextToClassify* text);

/**
 * Performs re-training of a new or re-training of an existing model. The training data are provided
 * in pTextToTrain.
 * */
void TextClassifier_Train(void* vThis, int nModel, TextToClassify* pTextToTrain);

/**
 * Prints information about a text classifier and the loaded models to the standard output.
 * */
void TextClassifier_PrintInfo(void* vThis);

#ifdef __cplusplus
}
#endif

#endif /* CLASSIFIER_H */
