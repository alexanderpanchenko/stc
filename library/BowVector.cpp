#include "BowVector.hpp"

BowVector::BowVector() {}

BowVector::~BowVector() { }

bool BowVector::increment_word(long id, long increment) {
	// Try to find existing entry
	map<long,long>::iterator it = find(id);
	if (it != end()){
		(*it).second = (*it).second + increment;
	}
	// Create new entry
	else {
		insert(make_pair(id, increment));
	}
	return true;
}

bool BowVector::increment_word(long id) {
	return increment_word(id, 1);
}

/**
 * Creates a string representing current BoW vector.
 * The vector representation can be either raw or
 * unit normalized if is_unit_length = true;
 * */
string BowVector::to_string(bool is_unit_length) {
	const char FEATURE_LENGTH = 50; // chars
	string feature_string; //libsvm feature vector
	double norm = (is_unit_length ? get_norm() : 1);

	map<long, long>::iterator it;
	for(it = begin(); it != end(); it++) {
	    char feature[FEATURE_LENGTH];
	    double value =  it->second / norm;
	    sprintf(feature, "%ld: %f ", it->first, value);
	    feature_string.append(feature);
	}

	return feature_string;
}


/**
 * Calculates L1 norm of the vector (euclidian norm).
 * norm = sqrt(sum_i(f_i^2))
 * */
double BowVector::get_norm(void) {
	double norm = 0;

	map<long, long>::iterator it;
	for(it = begin(); it != end(); it++) {
		norm += it->second * it->second;
	}
	norm = sqrt(norm);

	return norm;
}

/**
 * Multiply all values of the vector by a scalar
 * */
void BowVector::multiply(long scalar) {
	map<long, long>::iterator it;
	for(it = begin(); it != end(); it++) {
		it->second = it->second * scalar;
	}
}

void BowVector::add(BowVector* vector) {
	for(map<long,long>::iterator it = vector->begin(); it != vector->end(); it++) {
		increment_word(it->first, it->second);
	}
}

