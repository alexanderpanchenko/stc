#ifndef INIPARAM_H
#define INIPARAM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * These constants define the names of the paramters used by text classifier.
 * A parameter is in format NAME_N, where NAME is the name of the parameter, such as STOPWORDS,
 * and N is the number of the model e.g. 1.
 * */
#define KEY_STOPWORDS "STOPWORDS" /* path to a file with semantic relations e.g. "stopwords.csv" */
#define KEY_STOPOS "STOPOS" /* path to a file with semantic relations e.g. "stopos.csv" */
#define KEY_VOCABULARY "VOCABULARY" /* path to a file with semantic relations e.g. "vocabulary.csv" */
#define KEY_MODEL "MODEL" /* path to a file with semantic relations e.g. "model" */
#define KEY_RELATIONS "RELATIONS" /* path to a file with semantic relations e.g. "relations.csv" */
#define KEY_VERBOSE "VERBOSE" /* "true" of "false" */
#define INIPARAMS_NUM 6 /* Expected number of parameters per model (see above) */
#define INI_SEPARATOR "_" /* A separator between the NAME of the paramter and the N -- number of the model */

/**
 * A structure which used for initialization of the text classifier.
 * */
typedef struct _inparam inparam;
typedef struct _IniParam IniParam;
struct _IniParam {
    char* sKey; /* Should contain a key in the format "KEY_ID" */
    char* sVal; /* Should contain a value of the key */
    IniParam* pNext; /* A pointer to the next parameter or NULL for the last paramter. */
};


#ifdef __cplusplus
}
#endif

#endif /* INIPARAM_H */

