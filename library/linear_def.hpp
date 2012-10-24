#ifndef LINEAR_DEF_HPP_
#define LINEAR_DEF_HPP_

#include<list>

struct feature_node
{
	int index;
	double value;
};

struct problem
{
	int l, n;
	int *y;
	struct feature_node **x;
	double bias;            /* < 0 if no bias term */
};

enum { L2R_LR, L2R_L2LOSS_SVC_DUAL, L2R_L2LOSS_SVC, L2R_L1LOSS_SVC_DUAL, MCSVM_CS, L1R_L2LOSS_SVC, L1R_LR, L2R_LR_DUAL }; /* solver_type */

struct parameter
{
	int solver_type;

	/* these are for training only */
	double eps;	        /* stopping criteria */
	double C;
	int nr_weight;
	int *weight_label;
	double* weight;
};

struct model
{
	struct parameter param;
	int nr_class;		/* number of classes */
	int nr_feature;
	double *w;
	int *label;		/* label of each class */
	double bias;
};


std::list<int> predict_file(const char* vectors_file, const char* predict_file, struct model* classif_model);
int predict_main(int argc, char **argv);
int train_main(int argc, char **argv);
int train_fs(const char* input_file_name, const char* model_file_name);

#endif /* LINEAR_DEF_HPP_ */
