#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <list>
#include "linear.hpp"
#include "Classifier.hpp"

using namespace std;

struct feature_node *x;
int max_nr_attr = 64;

struct model* model_predict;
int flag_predict_probability=0;

void exit_input_error2(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	//exit(1);
}

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;
	
	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

list<int> do_predict(FILE *input, FILE *output, struct model* model_predict){
	list<int> labels_list;
	int correct = 0;
	int total = 0;

	int nr_class=get_nr_class(model_predict);
	double *prob_estimates=NULL;
	int j, n;
	int nr_feature=get_nr_feature(model_predict);
	if(model_predict->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;

	if(flag_predict_probability)
	{
		int *labels;

		if(!check_probability_model(model_predict))	{
			fprintf(stderr, "probability output is only supported for logistic regression\n");
			exit(1);
		}

		labels=(int *) malloc(nr_class*sizeof(int));
		get_labels(model_predict,labels);
		prob_estimates = (double *) malloc(nr_class*sizeof(double));
		fprintf(output,"labels");		
		for(j=0;j<nr_class;j++)
			fprintf(output," %d",labels[j]);
		fprintf(output,"\n");
		free(labels);
	}

	max_line_len = 1024;
	line = (char *)malloc(max_line_len*sizeof(char));
	while(readline(input) != NULL)
	{
		int i = 0;
		int target_label, predict_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = 0; // strtol gives 0 if wrong format

		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error2(total+1);

		target_label = (int) strtol(label,&endptr,10);
		if(endptr == label || *endptr != '\0')
			exit_input_error2(total+1);

		while(1)
		{
			if(i>=max_nr_attr-2)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct feature_node *) realloc(x,max_nr_attr*sizeof(struct feature_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
				exit_input_error2(total+1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error2(total+1);

			// feature indices larger than those in training are not used
			if(x[i].index <= nr_feature)
				++i;
		}

		if(model_predict->bias>=0)
		{
			x[i].index = n;
			x[i].value = model_predict->bias;
			i++;
		}
		x[i].index = -1;

		if(flag_predict_probability)
		{
			int j;
			predict_label = predict_probability(model_predict,x,prob_estimates);
			fprintf(output,"%d",predict_label);
			for(j=0;j<model_predict->nr_class;j++)
				fprintf(output," %g",prob_estimates[j]);
			fprintf(output,"\n");
		}
		else
		{
			predict_label = predict(model_predict,x);
			labels_list.push_back(predict_label);
			fprintf(output,"%d\n",predict_label);
		}

		if(predict_label == target_label)
			++correct;
		++total;
	}
	printf("Accuracy = %g%% (%d/%d)\n",(double) correct/total*100,correct,total);
	if(flag_predict_probability)
		free(prob_estimates);

	return labels_list;
}

void exit_with_help2()
{
	printf(
	"Usage: predict [options] test_file model_file output_file\n"
	"options:\n"
	"-b probability_estimates: whether to output probability estimates, 0 or 1 (default 0)\n"
	);
	exit(1);
}

int predict_main(int argc, char **argv)
{
	FILE *input, *output;
	int i;

	// parse options
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		++i;
		switch(argv[i-1][1])
		{
			case 'b':
				flag_predict_probability = atoi(argv[i]);
				break;

			default:
				fprintf(stderr,"unknown option: -%c\n", argv[i-1][1]);
				exit_with_help2();
				break;
		}
	}
	if(i>=argc)
		exit_with_help2();

	input = fopen(argv[i],"r");
	if(input == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",argv[i]);
		exit(1);
	}

	output = fopen(argv[i+2],"w");
	if(output == NULL)
	{
		fprintf(stderr,"can't open output file %s\n",argv[i+2]);
		exit(1);
	}

	if((model_predict=load_model(argv[i+1]))==0)
	{
		fprintf(stderr,"can't open model file %s\n",argv[i+1]);
		exit(1);
	}

	x = (struct feature_node *) malloc(max_nr_attr*sizeof(struct feature_node));
	do_predict(input, output, model_predict);
	free_and_destroy_model(&model_predict);
	free(line);
	free(x);
	fclose(input);
	fclose(output);
	return 0;
}

/*
void predict_fs(const char* vectors_file, const char* predict_file, struct model* classif_model){
//list<int> predict_fs(const char* vectors_file, const char* predict_file, struct model* classif_model){
	// Initialize the parameters
	list<int> labels_list;
	FILE *input, *output;
	input = fopen(vectors_file, "r");
	if(input == NULL){
		fprintf(stderr,"can't open input file %s\n", vectors_file);
		//return labels_list;
		return;
	}

	output = fopen(predict_file, "w");
	if(output == NULL){
		fprintf(stderr,"can't open output file %s\n", predict_file);
		//return labels_list;
		return;
	}

	if(classif_model == NULL){
		fprintf(stderr,"classification model is not loaded\n");
		//return labels_list;
		return;
	}
	model_predict = classif_model;

	// Predict the categories for the input data
	x = (struct feature_node *) malloc(max_nr_attr*sizeof(struct feature_node));
	labels_list = do_predict(input, output, model_predict);
	//free_and_destroy_model(&model_predict);
	free(line);
	free(x);
	fclose(input);
	fclose(output);
	//return labels_list;
	return;
}
*/

/**
 * Predicts the labels of texts
 * vectors_file 	Path to an input CSV file with vectors (one text per line) in LibSVM format
 * predict_file 	Path to the output file with predictions
 * model 			A model
 * */
list<int> predict_fs(const char* vectors_file, const char* predict_file, struct model* classif_model){
	// Initialize the parameters
	list<int> labels_list;
	FILE *input, *output;
	input = fopen(vectors_file, "r");
	if(input == NULL){
		fprintf(stderr,"can't open input file %s\n", vectors_file);
		return labels_list;
	}

	output = fopen(predict_file, "w");
	if(output == NULL){
		fprintf(stderr,"can't open output file %s\n", predict_file);
		return labels_list;
	}

	if(classif_model == NULL){
		fprintf(stderr,"classification model is not loaded\n");
		return labels_list;
	}
	model_predict = classif_model;

	// Predict the categories for the input data
	x = (struct feature_node *) malloc(max_nr_attr*sizeof(struct feature_node));
	labels_list = do_predict(input, output, model_predict);
	free(line);
	free(x);
	fclose(input);
	fclose(output);
	return labels_list;

}


