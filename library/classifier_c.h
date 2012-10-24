
#ifdef __cplusplus
extern "C" {
#endif

// Default names of the resources
#define DEFAULT_VOCABULARY_FILE "vocabulary.csv"
#define DEFAULT_STOPWORDS_FILE "stopwords.csv"
#define DEFAULT_STOPOS_FILE "stopos.csv"
#define DEFAULT_MODEL_FILE "model"
#define DEFAULT_RELATIONS_FILE "relations"


typedef struct ClassifierInfo{
    unsigned long uliInfoMode;
    const char* sProgName;
    const char* sResourcesDir;
}ClassifierInfo;

ClassifierInfo classifier_info;

/**
 * Creates the classifier and returns a pointer to it.
 * uliInfoMode 		Verbosity mode (MODE_QUIET etc.)
 * sProgName 		Name of the program
 * sResourcesDir 	Full path to a directory with resources required to the classifier (files "model", "vocabulary.csv", "stoplist.csv", "stopos.csv")
 * */
void* Classifier_Create(unsigned long uliInfoMode, const char* sProgName, const char* sResourcesDir);

/**
 * Destroys an instance of the classifier.
 * */
void  Classifier_Destroy(void* vThis);

/**
 * Reloads the classifier.
 * */
int   Classifier_Reload(void* vThis, char* sErrMsg);

/**
 * Classify a new XML file sFile
 * */
int   Classifier_FileProcess(void* vThis, const char* sFileName, unsigned long uliInfoMode);

/*
 * Classify a new XML file sFile in the server mode.
 */
int   Classifier_ServerProcess(void* vThis, char* sFileName, unsigned long uliInfoMode, char* sErrMsg);

#ifdef __cplusplus
}
#endif



